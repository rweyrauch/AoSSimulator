/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/Bloodreavers.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Bloodreavers::Create,
    Bloodreavers::ValueToString,
    Bloodreavers::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Bloodreavers::MIN_UNIT_SIZE, Bloodreavers::MIN_UNIT_SIZE,
            Bloodreavers::MAX_UNIT_SIZE, Bloodreavers::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", Bloodreavers::ReaverBlades, Bloodreavers::ReaverBlades,
            Bloodreavers::MeatripperAxe, 1
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblowers", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool Bloodreavers::s_registered = false;

Bloodreavers::Bloodreavers() :
    KhorneBase("Bloodreavers", 6, WOUNDS, 5, 6, false),
    m_reaverBlades(Weapon::Type::Melee, "Reaver Blades", 1, 1, 4, 4, 0, 1),
    m_reaverBladesChieftain(Weapon::Type::Melee, "Reaver Blades", 1, 2, 4, 4, 0, 1),
    m_meatripperAxe(Weapon::Type::Melee, "Meatripper Axe", 1, 1, 4, 4, -1, 1),
    m_meatripperAxeChieftain(Weapon::Type::Melee, "Meatripper Axe", 1, 2, 4, 4, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, BLOODREAVERS};
    m_weapons = {&m_reaverBlades, &m_reaverBladesChieftain, &m_meatripperAxe, &m_meatripperAxeChieftain};
}


bool Bloodreavers::configure(int numModels, Bloodreavers::WeaponOption weapons, bool iconBearer, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weaponOption = weapons;
    m_iconBearer = iconBearer;
    m_hornblower = hornblowers;

    // Add the Chieftain
    auto chieftainModel = new Model(BASESIZE, WOUNDS);
    if (m_weaponOption == ReaverBlades)
    {
        chieftainModel->addMeleeWeapon(&m_reaverBladesChieftain);
    }
    else if (m_weaponOption == MeatripperAxe)
    {
        chieftainModel->addMeleeWeapon(&m_meatripperAxeChieftain);
    }
    addModel(chieftainModel);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (m_weaponOption == ReaverBlades)
        {
            model->addMeleeWeapon(&m_reaverBlades);
        }
        else if (m_weaponOption == MeatripperAxe)
        {
            model->addMeleeWeapon(&m_meatripperAxe);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Rerolls Bloodreavers::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Reaver Blades
    if (m_weaponOption == ReaverBlades)
    {
        return RerollOnes;
    }

    return KhorneBase::toHitRerolls(weapon, unit);
}

int Bloodreavers::braveryModifier() const
{
    int modifier = KhorneBase::braveryModifier();

    // Icon Bearer
    if (m_iconBearer)
    {
        modifier++;
    }

    return modifier;
}

Unit *Bloodreavers::Create(const ParameterList &parameters)
{
    auto unit = new Bloodreavers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, ReaverBlades);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure(numModels, weapons, iconBearer, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bloodreavers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodreavers", factoryMethod);
    }
}

std::string Bloodreavers::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == ReaverBlades)
        { return "Reaver Blades"; }
        else if (parameter.m_intValue == MeatripperAxe)
        { return "Meatripper Axe"; }
    }
    return KhorneBase::ValueToString(parameter);
}

int Bloodreavers::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Reaver Blades")
    { return ReaverBlades; }
    else if (enumString == "Meatripper Axe")
    { return MeatripperAxe; }
    return KhorneBase::EnumStringToInt(enumString);
}

int Bloodreavers::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = KhorneBase::extraAttacks(nullptr, weapon, target);

    // Frenzied Devotion
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 16.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(KHORNE) && ip->hasKeyword(TOTEM))
        {
            attacks += 1;
            break;
        }
    }

    return attacks;
}

int Bloodreavers::runModifier() const
{
    int modifier = KhorneBase::runModifier();
    if (m_hornblower)
    {
        modifier += 1;
    }
    return modifier;
}

int Bloodreavers::chargeModifier() const
{
    int modifier = KhorneBase::chargeModifier();
    if (m_hornblower)
    {
        modifier += 1;
    }
    return modifier;
}

} // namespace Khorne