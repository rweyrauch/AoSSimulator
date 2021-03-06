/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/MelusaiIronscale.h>
#include <UnitFactory.h>
#include <Board.h>
#include "DaughterOfKhainePrivate.h"
#include "DoKCommands.h"

namespace DaughtersOfKhaine {

    class WrathOfTheScathborn : public CommandAbility {
    public:
        explicit WrathOfTheScathborn(Unit *general) :
                CommandAbility(general, "Wrath of the Scáthborn", 12, 12, GamePhase::Hero) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_targetKeywords = {MELUSAI};
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        bool apply(Unit *target) override {
            if (target == nullptr)
                return false;

            target->buffMovement(MovementRule::Run_And_Shoot, true, defaultDuration());
            target->buffMovement(MovementRule::Run_And_Charge, true, defaultDuration());
            target->buffModifier(Attribute::Run_Distance, Dice::RollD6(), defaultDuration());

            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 110;

    bool MelusaiIronscale::s_registered = false;

    MelusaiIronscale::MelusaiIronscale(Temple temple, CommandTrait trait, Artefact artefact, bool isGeneral) :
            DaughterOfKhaine(temple, "Melusai Ironscale", 6, g_wounds, 9, 5, false, g_pointsPerUnit),
            m_keldrisaithShooting(Weapon::Type::Missile, "Keldrisaith", 18, 3, 3, 3, -1, 1),
            m_keldrisaith(Weapon::Type::Melee, "Keldrisaith", 2, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, MELUSAI, HERO, MELUSAI_IRONSCALE};
        m_weapons = {&m_keldrisaithShooting, &m_keldrisaith};
        m_battleFieldRole = Role::Leader;

        // Pact of Blood
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_keldrisaithShooting);
        model->addMeleeWeapon(&m_keldrisaith);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<WrathOfTheScathborn>(this));
        configureCommon();
    }

    Unit *MelusaiIronscale::Create(const ParameterList &parameters) {
        auto temple = (Temple) GetEnumParam("Temple", parameters, g_temple[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_melusaiCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_priestArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new MelusaiIronscale(temple, trait, artefact, general);
    }

    void MelusaiIronscale::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Temple", g_temple[0], g_temple),
                            EnumParameter("Command Trait", g_melusaiCommandTraits[0], g_melusaiCommandTraits),
                            EnumParameter("Artefact", g_priestArtefacts[0], g_priestArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Melusai Ironscale", factoryMethod);
        }
    }

    int MelusaiIronscale::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds MelusaiIronscale::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        Wounds totalWounds = wounds;
        if (wounds.source == Wounds::Source::Spell) {
            if (Dice::RollD6() >= 5) {
                totalWounds.normal = 0;
                totalWounds.mortal = 0;
            }
        }
        return DaughterOfKhaine::applyWoundSave(totalWounds, attackingUnit);
    }

    void MelusaiIronscale::onEndCombat(PlayerId player) {
        DaughterOfKhaine::onEndCombat(player);

        // Turned to Crystal
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
            if (!units.empty()) {
                auto unit = units.front();
                Dice::RollResult result;
                Dice::RollD6(unit->remainingModels(), result);
                unit->applyDamage({0, result.rollsGE(3), Wounds::Source::Ability}, this);
            }
        }
    }

} //namespace DaughtersOfKhaine

