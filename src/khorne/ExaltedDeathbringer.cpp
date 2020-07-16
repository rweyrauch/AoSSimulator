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
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    bool ExaltedDeathbringer::s_registered = false;

    ExaltedDeathbringer::ExaltedDeathbringer() :
            KhorneBase("Exalted Deathbringer", 5, WOUNDS, 8, 4, false),
            m_ruinousAxe(Weapon::Type::Melee, "Ruinous Axe", 1, 3, 4, 3, -1, 2),
            m_bloodbiteAxe(Weapon::Type::Melee, "Bloodbite Axe", 1, 6, 3, 4, 0, 1),
            m_impalingSpear(Weapon::Type::Melee, "Impaling Spear", 2, 5, 3, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, EXALTED_DEATHBRINGER};
        m_weapons = {&m_ruinousAxe, &m_bloodbiteAxe, &m_impalingSpear};
        m_battleFieldRole = Leader;
    }

    bool ExaltedDeathbringer::configure(WeaponOption weapon) {
        m_weaponOption = weapon;

        auto model = new Model(BASESIZE, wounds());

        if (m_weaponOption == RuinousAxeAndSkullgouger) {
            model->addMeleeWeapon(&m_ruinousAxe);
        } else if (m_weaponOption == BloodbiteAxeAndRunemarkedShield) {
            model->addMeleeWeapon(&m_bloodbiteAxe);
        } else if (m_weaponOption == ImpalingSpear) {
            model->addMeleeWeapon(&m_impalingSpear);
        }
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *ExaltedDeathbringer::Create(const ParameterList &parameters) {
        auto unit = new ExaltedDeathbringer();
        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, RuinousAxeAndSkullgouger);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ExaltedDeathbringer::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {RuinousAxeAndSkullgouger, BloodbiteAxeAndRunemarkedShield, ImpalingSpear};
            static FactoryMethod factoryMethod = {
                    ExaltedDeathbringer::Create,
                    ExaltedDeathbringer::ValueToString,
                    ExaltedDeathbringer::EnumStringToInt,
                    ExaltedDeathbringer::ComputePoints,
                    {
                            EnumParameter("Weapon", RuinousAxeAndSkullgouger, weapons),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0], g_mortalbloodboundCommandTraits),
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
            if (parameter.intValue == RuinousAxeAndSkullgouger) {
                return "Ruinous Axe and Skullgouger";
            } else if (parameter.intValue == BloodbiteAxeAndRunemarkedShield) {
                return "Bloodbite Axe and Runemarked Shield";
            } else if (parameter.intValue == ImpalingSpear) {
                return "Impaling Spear";
            }
        }
        return KhorneBase::ValueToString(parameter);
    }

    int ExaltedDeathbringer::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ruinous Axe and Skullgouger") {
            return RuinousAxeAndSkullgouger;
        } else if (enumString == "Bloodbite Axe and Runemarked Shield") {
            return BloodbiteAxeAndRunemarkedShield;
        } else if (enumString == "Impaling Spear") {
            return ImpalingSpear;
        }
        return KhorneBase::EnumStringToInt(enumString);
    }

    int ExaltedDeathbringer::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

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

    Wounds ExaltedDeathbringer::applyWoundSave(const Wounds &wounds) {
        if (m_weaponOption == BloodbiteAxeAndRunemarkedShield) {
            auto totalWounds = KhorneBase::applyWoundSave(wounds);

            if (totalWounds.source == Wounds::Source::Spell) {
                Dice::RollResult result;
                Dice::rollD6(totalWounds.normal, result);
                totalWounds.normal -= result.rollsGE(2);
                Dice::rollD6(totalWounds.mortal, result);
                totalWounds.mortal -= result.rollsGE(2);
            }
            return totalWounds;
        }
        return KhorneBase::applyWoundSave(wounds);
    }

    Wounds
    ExaltedDeathbringer::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Brutal Impalement
        if ((woundRoll == 6) && (weapon->name() == m_impalingSpear.name())) {
            return {weapon->damage(), Dice::rollD3()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds ExaltedDeathbringer::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        // Skullgouger
        if ((saveRoll == 6) && (m_weaponOption == RuinousAxeAndSkullgouger)) {
            return {0, Dice::rollD3()};
        }
        return Unit::computeReturnedDamage(weapon, saveRoll);
    }

    int ExaltedDeathbringer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }


} // namespace Khorne