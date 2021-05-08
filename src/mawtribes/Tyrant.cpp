/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "mawtribes/Tyrant.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 50;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 160;

    bool Tyrant::s_registered = false;

    bool Tyrant::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *Tyrant::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto bigName = (BigName) GetEnumParam("Big Name", parameters, (int) BigName::Fateseeker);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_tyrantTraits[0]);
            auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_tyrantArtefacts[0]);
            auto general = GetBoolParam("General", parameters, false);
            return new Tyrant(tribe, bigName, trait, artefact, general);
        }
        return nullptr;
    }

    std::string Tyrant::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Big Name") {
            auto bigName = magic_enum::enum_name((BigName) parameter.intValue);
            return std::string(bigName);
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int Tyrant::EnumStringToInt(const std::string &enumString) {
        auto bigName = magic_enum::enum_cast<BigName>(enumString);
        if (bigName.has_value()) {
            return (int) bigName.value();
        }
        return MawtribesBase::EnumStringToInt(enumString);
    }

    void Tyrant::Init() {
        if (!s_registered) {
            static const std::array<int, 6> bignames = {(int) BigName::Deathcheater, (int) BigName::Brawlerguts,
                                                        (int) BigName::Fateseeker,
                                                        (int) BigName::Longstrider, (int) BigName::Giantbreaker,
                                                        (int) BigName::Wallcrusher};
            static FactoryMethod factoryMethod = {
                    Tyrant::Create,
                    Tyrant::ValueToString,
                    Tyrant::EnumStringToInt,
                    Tyrant::ComputePoints,
                    {
                            EnumParameter("Big Name", (int) BigName::Fateseeker, bignames),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_tyrantTraits[0], g_tyrantTraits),
                            EnumParameter("Artefact", g_tyrantArtefacts[0], g_tyrantArtefacts),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Tyrant", factoryMethod);
        }
    }

    Tyrant::Tyrant(Mawtribe tribe, Tyrant::BigName bigName, CommandTrait trait, Artefact artefact, bool isGeneral) :
            MawtribesBase(tribe, "Tyrant", 6, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, HERO, TYRANT};
        m_weapons = {&m_pistols, &m_thundermace, &m_glaive, &m_bite};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        m_bigName = bigName;

        // TODO: if trait == Killer_Reputation, select a second BigName.

        if ((m_bigName == BigName::Deathcheater) || (m_bigNameExtra == BigName::Deathcheater)) m_wounds = g_wounds + 1;
        if ((m_bigName == BigName::Fateseeker) || (m_bigNameExtra == BigName::Fateseeker)) m_save = 3;
        else if ((m_bigName == BigName::Longstrider) || (m_bigNameExtra == BigName::Longstrider)) m_move = 8;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_pistols);
        model->addMeleeWeapon(&m_thundermace);
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_bite);
        addModel(model);

        if (m_commandTrait == CommandTrait::Mighty_Bellower) {
            s_globalBattleshockFleeModifier.connect(this, &Tyrant::mightyBellower, &m_mightyBellower);
        }
    }

    Tyrant::~Tyrant() {
        m_mightyBellower.disconnect();
    }

    Wounds Tyrant::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if ((hitRoll == 6) && (weapon->name() == m_glaive.name())) {
            if (target->hasKeyword(HERO) || target->hasKeyword(MONSTER)) {
                return {Dice::RollD6(), 0};
            }
        }
        if ((hitRoll == 6) && (weapon->name() == m_thundermace.name())) {
            if (target->remainingModels() > 3) {
                return {weapon->damage(), Dice::RollD3()};
            } else {
                return {weapon->damage(), 1};
            }
        }
        return MawtribesBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Tyrant::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = MawtribesBase::toWoundModifier(weapon, target);

        if (m_charged && ((m_bigName == BigName::Brawlerguts) || (m_bigNameExtra == BigName::Brawlerguts))) mod++;

        return mod;
    }

    int Tyrant::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void Tyrant::onStartHero(PlayerId player) {
        MawtribesBase::onStartHero(player);

        if (owningPlayer() == player) {
            if (m_commandTrait == CommandTrait::Furious_Guzzler) {
                auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
                const bool isEating = !units.empty();
                if (isEating) {
                    heal(Dice::RollD3());
                }
            }
        }
    }

    int Tyrant::woundModifier() const {
        auto mod = MawtribesBase::woundModifier();
        if (m_commandTrait == CommandTrait::Prodigious_Girth) {
            mod += 2;
        }
        return mod;
    }

    int Tyrant::mightyBellower(const Unit *unit, int roll) {
        if (distanceTo(unit) < 6.0) {
            return Dice::RollD3();
        }
        return 0;
    }

    Rerolls Tyrant::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->isMelee() && target->hasKeyword(Has_Artefact) &&
            (m_commandTrait == CommandTrait::An_Eye_For_Loot)) {
            return Rerolls::Failed;
        }
        return MawtribesBase::toHitRerolls(weapon, target);
    }

    Rerolls Tyrant::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->isMelee() && target->hasKeyword(Has_Artefact) &&
            (m_commandTrait == CommandTrait::An_Eye_For_Loot)) {
            return Rerolls::Failed;
        }
        return MawtribesBase::toWoundRerolls(weapon, target);
    }

} // namespace OgorMawtribes
