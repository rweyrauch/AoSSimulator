/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/Tzaangors.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{
static FactoryMethod factoryMethod = {
    Tzaangors::Create,
    Tzaangors::ValueToString,
    Tzaangors::EnumStringToInt,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Tzaangors::MIN_UNIT_SIZE}, Tzaangors::MIN_UNIT_SIZE,
            Tzaangors::MAX_UNIT_SIZE, Tzaangors::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "weapons", {.m_intValue = Tzaangors::PairedSavageBlades}, Tzaangors::PairedSavageBlades,
            Tzaangors::SavageBladeAndShield, 1
        },
        {ParamType::Integer, "numGreatblades", {.m_intValue = 0}, 0, Tzaangors::MAX_UNIT_SIZE/Tzaangors::MIN_UNIT_SIZE*2, 1},
        {ParamType::Integer, "numMutants", {.m_intValue = 0}, 0, Tzaangors::MAX_UNIT_SIZE/Tzaangors::MIN_UNIT_SIZE, 1},
        {ParamType::Boolean, "iconBearer", {.m_boolValue = false}, false, false, 1},
        {ParamType::Boolean, "brayhorns", {.m_boolValue = false}, false, false, 1},
    },
    CHAOS,
    TZEENTCH
};

bool Tzaangors::s_registered = false;

Tzaangors::Tzaangors() :
    Unit("Tzaangors", 6, WOUNDS, 5, 5, false),
    m_savageBlade(Weapon::Type::Melee, "Savage Blade", 1, 2, 4, 4, 0, 1),
    m_savageBladeTwistbray(Weapon::Type::Melee, "Savage Blade (Twistbray)", 1, 2, 3, 4, 0, 1),
    m_savageGreatblade(Weapon::Type::Melee, "Savage Greatblade", 1, 1, 4, 4, -1, 2),
    m_savageGreatbladeTwistbray(Weapon::Type::Melee, "Savage Greatblade (Twistbray)", 1, 1, 3, 4, -1, 2),
    m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
    m_viciousBeakTwistbray(Weapon::Type::Melee, "Vicious Beak (Twistbray)", 1, 1, 3, 5, 0, 1)
{
    m_keywords = {CHAOS, GOR, TZEENTCH, ARCANITE, TZAANGORS};
}

bool Tzaangors::configure(int numModels, WeaponOptions weapons, int numGreatblades, int numMutants, bool iconBearer, bool brayhorns)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    const int maxGreatblades = (numGreatblades / MIN_UNIT_SIZE) * 2;
    if (numGreatblades > maxGreatblades)
    {
        return false;
    }
    const int maxMutents = numModels / MIN_UNIT_SIZE;
    if (numMutants > maxMutents)
    {
        return false;
    }

    m_weapons = weapons;
    m_numGreatblades = numGreatblades;
    m_numMutants = numMutants;
    m_iconBearer = iconBearer;
    m_brayhorns = brayhorns;

    // Brayhorns
    m_runAndCharge = brayhorns;

    Model twistbray(BASESIZE, WOUNDS);
    if (numGreatblades)
        twistbray.addMeleeWeapon(&m_savageGreatbladeTwistbray);
    else
        twistbray.addMeleeWeapon(&m_savageBladeTwistbray);
    twistbray.addMeleeWeapon(&m_viciousBeakTwistbray);
    addModel(twistbray);

    // the Twistbray is always given a greatblade if there are any greatblades
    for (auto i = 1; i < numGreatblades; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_savageGreatblade);
        model.addMeleeWeapon(&m_viciousBeak);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_savageBlade);
        model.addMeleeWeapon(&m_viciousBeak);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Tzaangors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_savageBlade);
    visitor(&m_savageBladeTwistbray);
    visitor(&m_savageGreatblade);
    visitor(&m_savageGreatbladeTwistbray);
    visitor(&m_viciousBeak);
    visitor(&m_viciousBeakTwistbray);
}

Unit *Tzaangors::Create(const ParameterList &parameters)
{
    auto unit = new Tzaangors();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOptions)GetEnumParam("weapons", parameters, SavageBladeAndShield);
    int numGreatblades = GetIntParam("numGreatblades", parameters, 0);
    int numMutants = GetIntParam("numMutants", parameters, 0);
    bool iconBearer = GetBoolParam("iconBearer", parameters, false);
    bool brayhorns = GetBoolParam("brayhorns", parameters, false);

    bool ok = unit->configure(numModels, weapons, numGreatblades, numMutants, iconBearer, brayhorns);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Tzaangors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Tzaangors", factoryMethod);
    }
}

std::string Tzaangors::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == SavageBladeAndShield)
        {
            return "SavageBladeAndShield";
        }
        else if (parameter.m_intValue == PairedSavageBlades)
        {
            return "PairedSavageBlades";
        }
    }

    return ParameterValueToString(parameter);
}

int Tzaangors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "SavageBladeAndShield")
    {
        return SavageBladeAndShield;
    }
    else if (enumString == "PairedSavageBlades")
    {
        return PairedSavageBlades;
    }
    return 0;
}

Wounds Tzaangors::applyWoundSave(const Wounds &wounds)
{
    // Arcanite Shield
    if (m_weapons == SavageBladeAndShield)
    {
        Dice dice;
        auto roll = dice.rollD6();
        if (roll == 6)
            return {0, 0};
    }
    return Unit::applyWoundSave(wounds);
}

int Tzaangors::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);

    // Paired Savage Blades
    if ((m_weapons == PairedSavageBlades) && (weapon->name() == m_savageBlade.name()))
        modifier += 1;

    return modifier;
}

int Tzaangors::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toWoundModifier(weapon, target);

    // Anarchy and Mayhem
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 9.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(ARCANITE) && ip->hasKeyword(HERO))
        {
            modifier += 1;
            break;
        }
    }

    return modifier;
}

int Tzaangors::extraAttacks(const Weapon *weapon) const
{
    int attacks = Unit::extraAttacks(weapon);

    // Savagery Unleashed
    if (weapon->name() == m_savageBlade.name() || weapon->name() == m_savageGreatblade.name())
    {
        attacks += std::max(3, remainingModels() / 9);
    }
    return attacks;
}

} //namespace Tzeentch