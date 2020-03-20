/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Slaughterpriest.h>
#include <UnitFactory.h>

namespace Khorne
{
static const int BASESIZE = 40;
static const int WOUNDS = 6;
static const int POINTS_PER_UNIT = 100;

bool Slaughterpriest::s_registered = false;

Slaughterpriest::Slaughterpriest() :
    KhorneBase("Slaughterpriest", 6, WOUNDS, 8, 5, false),
    m_bloodbathedAxe(Weapon::Type::Melee, "Bloodbathed Axe", 2, 3, 4, 3, 0, 2),
    m_hackblade(Weapon::Type::Melee, "Hackblade", 1, 3, 3, 4, 0, 1),
    m_wrathHammer(Weapon::Type::Melee, "Wrath-hammer", 3, RAND_D3, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, PRIEST, SLAUGHTERPRIEST};
    m_weapons = {&m_bloodbathedAxe, &m_hackblade, &m_wrathHammer};

    // Scorn of Sorcery
    m_totalUnbinds = 1;

    m_totalPrayers = 2;
}

bool Slaughterpriest::configure(WeaponOption weapon, BloodBlessingsOfKhorne blessing)
{
    auto model = new Model(BASESIZE, wounds());

    if (weapon == BloodbathedAxe)
    {
        model->addMeleeWeapon(&m_bloodbathedAxe);
    }
    else
    {
        model->addMeleeWeapon(&m_hackblade);
        model->addMeleeWeapon(&m_wrathHammer);
    }
    addModel(model);

    m_knownPrayers.push_back(std::unique_ptr<Prayer>(CreateBloodBlessingsOfKhorne(blessing, this)));
    m_knownPrayers.push_back(std::unique_ptr<Prayer>(new DamagePrayer(this, "Blood Boil", 4, 16.0f, RAND_D6, RAND_D3)));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Slaughterpriest::Create(const ParameterList &parameters)
{
    auto unit = new Slaughterpriest();
    WeaponOption weapon = (WeaponOption) GetEnumParam("Weapon", parameters, BloodbathedAxe);
    auto blessing = (BloodBlessingsOfKhorne) GetEnumParam("Blood Blessings of Khorne", parameters, (int)BloodBlessingsOfKhorne::None);

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure(weapon, blessing);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Slaughterpriest::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Slaughterpriest::Create,
            Slaughterpriest::ValueToString,
            Slaughterpriest::EnumStringToInt,
            Slaughterpriest::ComputePoints,
            {
                {
                    ParamType::Enum, "Weapon", Slaughterpriest::BloodbathedAxe, Slaughterpriest::BloodbathedAxe,
                    Slaughterpriest::HackbladeAndWrathHammer, 1
                },
                {
                    ParamType::Enum, "Blood Blessings of Khorne", (int)BloodBlessingsOfKhorne::BronzedFlesh,
                    (int)BloodBlessingsOfKhorne::None, (int)BloodBlessingsOfKhorne::SpellbaneHex, 1
                },
                {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
            },
            CHAOS,
            { KHORNE }
        };
        s_registered = UnitFactory::Register("Slaughterpriest", factoryMethod);
    }
}

std::string Slaughterpriest::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == BloodbathedAxe) { return "Bloodbathed Axe"; }
        else if (parameter.intValue == HackbladeAndWrathHammer) { return "Hackblade and Wrath-hammer"; }
    }
    else if (std::string(parameter.name) == "Blood Blessings of Khorne")
    {
        return ToString((BloodBlessingsOfKhorne)parameter.intValue);
    }
    return KhorneBase::ValueToString(parameter);
}

int Slaughterpriest::EnumStringToInt(const std::string &enumString)
{
    BloodBlessingsOfKhorne blessing;
    if (FromString(enumString, blessing))
    {
        return (int) blessing;
    }

    if (enumString == "Bloodbathed Axe") { return BloodbathedAxe; }
    else if (enumString == "Hackblade and Wrath-hammer") { return HackbladeAndWrathHammer; }
    return KhorneBase::EnumStringToInt(enumString);
}

int Slaughterpriest::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace Khorne