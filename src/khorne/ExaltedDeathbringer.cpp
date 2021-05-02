/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/ExaltedDeathbringer.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool ExaltedDeathbringer::s_registered = false;

    ExaltedDeathbringer::ExaltedDeathbringer(SlaughterHost host, WeaponOption weapon, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Exalted Deathbringer", 5, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, EXALTED_DEATHBRINGER};
        m_weapons = {&m_ruinousAxe, &m_bloodbiteAxe, &m_impalingSpear};
        m_battleFieldRole = Role::Leader;

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        m_weaponOption = weapon;

        auto model = new Model(g_basesize, wounds());

        if (m_weaponOption == Ruinous_Axe_And_Skullgouger) {
            model->addMeleeWeapon(&m_ruinousAxe);
        } else if (m_weaponOption == Bloodbite_Axe_And_Runemarked_Shield) {
            model->addMeleeWeapon(&m_bloodbiteAxe);
        } else if (m_weaponOption == Impaling_Spear) {
            model->addMeleeWeapon(&m_impalingSpear);
        }
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffAbilityCommandAbility>(this, "Brutal Command", 18, 18, GamePhase::Battleshock,
                                                            Ability::Ignore_Battleshock,
                                                            1, Abilities::Target::Friendly,
                                                            std::vector<Keyword>{MORTAL, KHORNE}));

        m_points = g_pointsPerUnit;
    }

    Unit *ExaltedDeathbringer::Create(const ParameterList &parameters) {
        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Ruinous_Axe_And_Skullgouger);
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

       return new ExaltedDeathbringer(host, weapon, trait, artefact, general);
    }

    void ExaltedDeathbringer::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Ruinous_Axe_And_Skullgouger, Bloodbite_Axe_And_Runemarked_Shield,
                                                       Impaling_Spear};
            static FactoryMethod factoryMethod = {
                    ExaltedDeathbringer::Create,
                    ExaltedDeathbringer::ValueToString,
                    ExaltedDeathbringer::EnumStringToInt,
                    ExaltedDeathbringer::ComputePoints,
                    {
                            EnumParameter("Weapon", Ruinous_Axe_And_Skullgouger, weapons),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0],
                                          g_mortalbloodboundCommandTraits),
                            EnumParameter("Artefact", g_mortalArtefacts[0], g_mortalArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Exalted Deathbringer", factoryMethod);
        }
    }

    std::string ExaltedDeathbringer::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Ruinous_Axe_And_Skullgouger) {
                return "Ruinous Axe and Skullgouger";
            } else if (parameter.intValue == Bloodbite_Axe_And_Runemarked_Shield) {
                return "Bloodbite Axe and Runemarked Shield";
            } else if (parameter.intValue == Impaling_Spear) {
                return "Impaling Spear";
            }
        }
        return KhorneBase::ValueToString(parameter);
    }

    int ExaltedDeathbringer::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ruinous Axe and Skullgouger") {
            return Ruinous_Axe_And_Skullgouger;
        } else if (enumString == "Bloodbite Axe and Runemarked Shield") {
            return Bloodbite_Axe_And_Runemarked_Shield;
        } else if (enumString == "Impaling Spear") {
            return Impaling_Spear;
        }
        return KhorneBase::EnumStringToInt(enumString);
    }

    int ExaltedDeathbringer::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = KhorneBase::extraAttacks(attackingModel, weapon, target);

        // Blooded Lieutenant
        if (!isGeneral()) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit : units) {
                if (unit->isGeneral()) {
                    extra += 2;
                    break;
                }
            }
        }

        return extra;
    }

    Wounds ExaltedDeathbringer::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        if (m_weaponOption == Bloodbite_Axe_And_Runemarked_Shield) {
            auto totalWounds = KhorneBase::applyWoundSave(wounds, attackingUnit);
            if (totalWounds.source == Wounds::Source::Spell) {
                totalWounds = ignoreWounds(totalWounds, 2);
            }
            return totalWounds;
        }
        return KhorneBase::applyWoundSave(wounds, attackingUnit);
    }

    Wounds
    ExaltedDeathbringer::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Brutal Impalement
        if ((woundRoll == 6) && (weapon->name() == m_impalingSpear.name())) {
            return {weapon->damage(), Dice::RollD3()};
        }
        return KhorneBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Wounds ExaltedDeathbringer::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        // Skullgouger
        if ((saveRoll == 6) && (m_weaponOption == Ruinous_Axe_And_Skullgouger)) {
            return {0, Dice::RollD3()};
        }
        return KhorneBase::computeReturnedDamage(weapon, saveRoll);
    }

    int ExaltedDeathbringer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }


} // namespace Khorne