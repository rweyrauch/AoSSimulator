/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Slaughterpriest.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 100;

    bool Slaughterpriest::s_registered = false;

    Slaughterpriest::Slaughterpriest() :
            KhorneBase("Slaughterpriest", 6, g_wounds, 8, 5, false),
            m_bloodbathedAxe(Weapon::Type::Melee, "Bloodbathed Axe", 2, 3, 4, 3, 0, 2),
            m_hackblade(Weapon::Type::Melee, "Hackblade", 1, 3, 3, 4, 0, 1),
            m_wrathHammer(Weapon::Type::Melee, "Wrath-hammer", 3, RAND_D3, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, PRIEST, SLAUGHTERPRIEST};
        m_weapons = {&m_bloodbathedAxe, &m_hackblade, &m_wrathHammer};
        m_battleFieldRole = Leader;

        // Scorn of Sorcery
        m_totalUnbinds = 1;

        m_totalPrayers = 2;
    }

    bool Slaughterpriest::configure(WeaponOption weapon, BloodBlessingsOfKhorne blessing) {
        auto model = new Model(g_basesize, wounds());

        if (weapon == Bloodbathed_Axe) {
            model->addMeleeWeapon(&m_bloodbathedAxe);
        } else {
            model->addMeleeWeapon(&m_hackblade);
            model->addMeleeWeapon(&m_wrathHammer);
        }
        addModel(model);

        m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateBloodBlessingsOfKhorne(blessing, this)));
        m_knownPrayers.push_back(
                std::unique_ptr<Prayer>(new DamagePrayer(this, "Blood Boil", 4, 16.0, RAND_D6, RAND_D3)));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Slaughterpriest::Create(const ParameterList &parameters) {
        auto unit = new Slaughterpriest();
        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Bloodbathed_Axe);
        auto blessing = (BloodBlessingsOfKhorne) GetEnumParam("Blood Blessings of Khorne", parameters,
                                                              g_bloodBlessingsOfKhorne[0]);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapon, blessing);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Slaughterpriest::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Bloodbathed_Axe, Hackblade_And_Wrath_Hammer};
            static FactoryMethod factoryMethod = {
                    Slaughterpriest::Create,
                    Slaughterpriest::ValueToString,
                    Slaughterpriest::EnumStringToInt,
                    Slaughterpriest::ComputePoints,
                    {
                            EnumParameter("Weapon", Bloodbathed_Axe, weapons),
                            EnumParameter("Blood Blessings of Khorne", g_bloodBlessingsOfKhorne[0], g_bloodBlessingsOfKhorne),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0], g_mortalbloodboundCommandTraits),
                            EnumParameter("Artefact", g_mortalArtefacts[0], g_mortalArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Slaughterpriest", factoryMethod);
        }
    }

    std::string Slaughterpriest::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Bloodbathed_Axe) { return "Bloodbathed Axe"; }
            else if (parameter.intValue == Hackblade_And_Wrath_Hammer) { return "Hackblade and Wrath-hammer"; }
        } else if (std::string(parameter.name) == "Blood Blessings of Khorne") {
            return ToString((BloodBlessingsOfKhorne) parameter.intValue);
        }
        return KhorneBase::ValueToString(parameter);
    }

    int Slaughterpriest::EnumStringToInt(const std::string &enumString) {
        BloodBlessingsOfKhorne blessing;
        if (FromString(enumString, blessing)) {
            return (int) blessing;
        }

        if (enumString == "Bloodbathed Axe") { return Bloodbathed_Axe; }
        else if (enumString == "Hackblade and Wrath-hammer") { return Hackblade_And_Wrath_Hammer; }
        return KhorneBase::EnumStringToInt(enumString);
    }

    int Slaughterpriest::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne