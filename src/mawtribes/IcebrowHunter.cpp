/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "mawtribes/IcebrowHunter.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 50;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 120;

    bool IcebrowHunter::s_registered = false;

    bool IcebrowHunter::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *IcebrowHunter::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_icebrowTraits[0]);
            auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_icebrowArtefacts[0]);
            auto general = GetBoolParam("General", parameters, false);
            return new IcebrowHunter(tribe, trait, artefact, general);
        }
        return nullptr;
    }

    void IcebrowHunter::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    IcebrowHunter::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    IcebrowHunter::ComputePoints,
                    {
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_icebrowTraits[0], g_icebrowTraits),
                            EnumParameter("Artefact", g_icebrowArtefacts[0], g_icebrowArtefacts),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Icebrow Hunter", factoryMethod);
        }
    }

    IcebrowHunter::IcebrowHunter(Mawtribe tribe, CommandTrait trait, Artefact artefact, bool isGeneral) :
            MawtribesBase(tribe, "Icebrow Hunter", 6, g_wounds, 7, 5, false) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, HERO, ICEBROW_HUNTER};
        m_weapons = {&m_spear, &m_crossbow, &m_club, &m_bite};
        m_battleFieldRole = Role::Leader;

        // Mighty Throw
        m_runAndShoot = true;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_spear);
        model->addMissileWeapon(&m_crossbow);
        model->addMeleeWeapon(&m_club);
        model->addMeleeWeapon(&m_bite);

        addModel(model);

        if (trait == CommandTrait::Raised_By_Yhetees) {
            s_globalAttackMod.connect(this, &IcebrowHunter::raisedByYhetees, &m_raisedByYhetees);
        }

        m_commandAbilities.push_back(
                std::make_unique<BuffModifierCommandAbility>(this, "Lead the Skal", 12, 12, GamePhase::Combat,
                                                             Attribute::Attacks_Melee, 1, Abilities::Target::Friendly,
                                                             std::vector<Keyword>{FROST_SABRES}));

        m_points = ComputePoints(1);
    }

    IcebrowHunter::~IcebrowHunter() {
        m_raisedByYhetees.disconnect();
    }

    int IcebrowHunter::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int IcebrowHunter::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        if (m_commandTrait == CommandTrait::Eye_Of_The_Blizzard) {
            mod--;
        }
        return mod;
    }

    int IcebrowHunter::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        if (weapon->isMissile() && (m_commandTrait == CommandTrait::Blood_Vultures_Gaze)) {
            mod++;
        }
        return mod;
    }

    int IcebrowHunter::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        if (weapon->isMissile() && (m_commandTrait == CommandTrait::Blood_Vultures_Gaze)) {
            mod++;
        }
        return mod;
    }

    void IcebrowHunter::onRan() {
        MawtribesBase::onRan();

        m_spear.setRange(18);
        m_spear.setDamage(RAND_D6);
    }

    void IcebrowHunter::onStartMovement(PlayerId player) {
        MawtribesBase::onStartMovement(player);

        m_spear.setRange(12);
        m_spear.setDamage(RAND_D3);
    }

    void IcebrowHunter::onStartShooting(PlayerId player) {
        MawtribesBase::onStartShooting(player);

        if (owningPlayer() == player) {
            m_spear.activate(true);
            m_crossbow.activate(true);

            // Icy Breath
            if (m_commandTrait == CommandTrait::Frost_Maw) {
                auto numUnits = Dice::RollD3();
                auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0);
                m_spear.activate(false);
                m_crossbow.activate(false);
                if (!units.empty()) {
                    for (auto unit : units) {
                        if (unit->remainingModels() > 0) {
                            if (Dice::RollD6() >= 4) {
                                unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                            }
                            numUnits--;
                            if (numUnits <= 0) {
                                break;
                            }
                        }
                    }
                }
            } else {
                if (shootingTarget() && (shootingTarget()->remainingModels() > 0) &&
                    distanceTo(shootingTarget()) < 6.0) {
                    m_spear.activate(false);
                    m_crossbow.activate(false);
                    if (Dice::RollD6() >= 4) {
                        shootingTarget()->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                    }
                }
            }
        }
    }

} // namespace OgorMawtribes
