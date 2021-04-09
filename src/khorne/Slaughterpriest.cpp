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

    Slaughterpriest::Slaughterpriest(SlaughterHost host, WeaponOption weapon, BloodBlessingsOfKhorne blessing, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Slaughterpriest", 6, g_wounds, 8, 5, false) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, PRIEST, SLAUGHTERPRIEST};
        m_weapons = {&m_bloodbathedAxe, &m_hackblade, &m_wrathHammer};
        m_battleFieldRole = Role::Leader;

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        // Scorn of Sorcery
        m_totalUnbinds = 1;

        m_totalPrayers = 2;

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
                std::unique_ptr<Prayer>(new DamagePrayer(this, "Blood Boil", 4, 16, RAND_D6, RAND_D3)));

        m_points = g_pointsPerUnit;
    }

    Unit *Slaughterpriest::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Bloodbathed_Axe);
        auto blessing = (BloodBlessingsOfKhorne) GetEnumParam("Blood Blessings of Khorne", parameters,
                                                              g_bloodBlessingsOfKhorne[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Slaughterpriest(host, weapon, blessing, trait, artefact, general);
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
                            EnumParameter("Blood Blessings of Khorne", g_bloodBlessingsOfKhorne[0],
                                          g_bloodBlessingsOfKhorne),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0],
                                          g_mortalbloodboundCommandTraits),
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