/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/MightySkullcrushers.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    MightySkullcrushers::Create,
    MightySkullcrushers::ValueToString,
    MightySkullcrushers::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", MightySkullcrushers::MIN_UNIT_SIZE, MightySkullcrushers::MIN_UNIT_SIZE,
            MightySkullcrushers::MAX_UNIT_SIZE, MightySkullcrushers::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", MightySkullcrushers::EnsorcelledAxe, MightySkullcrushers::EnsorcelledAxe,
            MightySkullcrushers::Bloodglaive, 1
        },
        {ParamType::Boolean, "Standard Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblowers", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    { KHORNE }
};

bool MightySkullcrushers::s_registered = false;

MightySkullcrushers::MightySkullcrushers() :
    KhorneBase("Mighty Skullcrushers", 8, WOUNDS, 6, 3, false),
    m_ensorcelledAxe(Weapon::Type::Melee, "Ensorcelled Axe", 1, 3, 3, 3, 0, 1),
    m_bloodglaive(Weapon::Type::Melee, "Bloodglaive", 1, 3, 4, 3, -1, 1),
    m_ensorcelledAxeHunter(Weapon::Type::Melee, "Ensorcelled Axe", 1, 4, 3, 3, 0, 1),
    m_bloodglaiveHunter(Weapon::Type::Melee, "Bloodglaive", 1, 4, 4, 3, -1, 1),
    m_brazenHooves(Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, MIGHTY_SKULLCRUSHERS};
    m_weapons = {&m_ensorcelledAxe, &m_bloodglaive, &m_ensorcelledAxeHunter, &m_bloodglaiveHunter, &m_brazenHooves};
}

bool MightySkullcrushers::configure(int numModels, WeaponOption weapons, bool standardBearer, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weaponOption = weapons;
    m_standarBearer = standardBearer;
    m_hornblower = hornblowers;

    // Add the Skullhunter
    auto hunter = new Model(BASESIZE, WOUNDS);
    if (m_weaponOption == EnsorcelledAxe)
    {
        hunter->addMeleeWeapon(&m_ensorcelledAxeHunter);
    }
    else if (m_weaponOption == Bloodglaive)
    {
        hunter->addMeleeWeapon(&m_bloodglaiveHunter);
    }
    hunter->addMeleeWeapon(&m_brazenHooves);
    addModel(hunter);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (m_weaponOption == EnsorcelledAxe)
        {
            model->addMeleeWeapon(&m_ensorcelledAxe);
        }
        else if (m_weaponOption == Bloodglaive)
        {
            model->addMeleeWeapon(&m_bloodglaive);
        }
        model->addMeleeWeapon(&m_brazenHooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *MightySkullcrushers::Create(const ParameterList &parameters)
{
    auto unit = new MightySkullcrushers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, EnsorcelledAxe);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure(numModels, weapons, standardBearer, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void MightySkullcrushers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Mighty Skullcrushers", factoryMethod);
    }
}

int MightySkullcrushers::chargeModifier() const
{
    int modifier = KhorneBase::chargeModifier();
    if (m_hornblower)
        modifier += 1;
    return modifier;
}

int MightySkullcrushers::runModifier() const
{
    int modifier = KhorneBase::runModifier();
    if (m_hornblower)
        modifier += 1;
    return modifier;
}

int MightySkullcrushers::braveryModifier() const
{
    int modifier = KhorneBase::braveryModifier();
    if (m_standarBearer)
        modifier += 2;
    return modifier;
}

std::string MightySkullcrushers::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == EnsorcelledAxe) { return "Ensorcelled Axe"; }
        else if (parameter.intValue == Bloodglaive) { return "Bloodglaive"; }
    }
    return KhorneBase::ValueToString(parameter);
}

int MightySkullcrushers::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Ensorcelled Axe") { return EnsorcelledAxe; }
    else if (enumString == "Bloodglaive") { return Bloodglaive; }
    return KhorneBase::EnumStringToInt(enumString);
}

void MightySkullcrushers::onCharged()
{
    // Murderous Charge
    if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0f))
    {
        Dice dice;
        Dice::RollResult rolls;
        dice.rollD6(remainingModels(), rolls);
        Wounds wounds = {0, 0};
        if (remainingModels() >= 6)
        {
            for (int i = 0; i < rolls.rollsGE(2); i++)
            {
                wounds.mortal += dice.rollD3();
            }
        }
        else
        {
            wounds.mortal = rolls.rollsGE(2);
        }

        SimLog(Verbosity::Narrative, "%s Murderous Charge inflicted %d mortal wounds on %s\n",
            name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

        m_meleeTarget->applyDamage(wounds);
    }
    KhorneBase::onCharged();
}

} // namespace Khorne
