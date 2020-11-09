/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/AspiringDeathbringer.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;
    static const int POINTS_PER_UNIT_WITH_GOREAXE = 100;

    bool AspiringDeathbringer::s_registered = false;

    AspiringDeathbringer::AspiringDeathbringer() :
            KhorneBase("Aspiring Deathbringer", 5, g_wounds, 7, 4, false),
            m_bloodAxe(Weapon::Type::Melee, "Bloodaxe", 1, 3, 3, 4, 0, 1),
            m_wrathHammer(Weapon::Type::Melee, "Wrath-hammer", 3, RAND_D3, 3, 4, 0, 1),
            m_goreaxe(Weapon::Type::Melee, "Goreaxe", 1, 3, 3, 4, 0, 1),
            m_skullhammer(Weapon::Type::Melee, "Skullhammer", 3, 3, 4, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, ASPIRING_DEATHBRINGER};
        m_weapons = {&m_bloodAxe, &m_wrathHammer, &m_goreaxe, &m_skullhammer};
        m_battleFieldRole = Leader;
    }

    bool AspiringDeathbringer::configure(WeaponOption weapon) {
        m_weaponOption = weapon;

        auto model = new Model(g_basesize, wounds());

        if (m_weaponOption == Bloodaxe_And_Wrathhammer) {
            model->addMeleeWeapon(&m_bloodAxe);
            model->addMeleeWeapon(&m_wrathHammer);
            m_points = g_pointsPerUnit;
        } else if (m_weaponOption == Goreaxe_And_Skullhammer) {
            model->addMeleeWeapon(&m_goreaxe);
            model->addMeleeWeapon(&m_skullhammer);
            m_points = POINTS_PER_UNIT_WITH_GOREAXE;
        }
        addModel(model);

        return true;
    }

    Unit *AspiringDeathbringer::Create(const ParameterList &parameters) {
        auto unit = new AspiringDeathbringer();
        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Bloodaxe_And_Wrathhammer);

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

    void AspiringDeathbringer::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Bloodaxe_And_Wrathhammer, Goreaxe_And_Skullhammer};
            static FactoryMethod factoryMethod = {
                    AspiringDeathbringer::Create,
                    AspiringDeathbringer::ValueToString,
                    AspiringDeathbringer::EnumStringToInt,
                    AspiringDeathbringer::ComputePoints,
                    {
                            EnumParameter("Weapon", Bloodaxe_And_Wrathhammer, weapons),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0], g_mortalbloodboundCommandTraits),
                            EnumParameter("Artefact", g_mortalArtefacts[0], g_mortalArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Aspiring Deathbringer", factoryMethod);
        }
    }

    std::string AspiringDeathbringer::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Bloodaxe_And_Wrathhammer) {
                return "Bloodaxe and Wrath-hammer";
            } else if (parameter.intValue == Goreaxe_And_Skullhammer) {
                return "Goreaxe and Skullhammer";
            }
        }
        return KhorneBase::ValueToString(parameter);
    }

    int AspiringDeathbringer::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Bloodaxe and Wrath-hammer") {
            return Bloodaxe_And_Wrathhammer;
        } else if (enumString == "Goreaxe and Skullhammer") {
            return Goreaxe_And_Skullhammer;
        }
        return KhorneBase::EnumStringToInt(enumString);
    }

    int AspiringDeathbringer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }


} // namespace Khorne