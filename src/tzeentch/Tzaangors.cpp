/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/Tzaangors.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{

bool Tzaangors::s_registered = false;

Tzaangors::Tzaangors() :
    TzeentchBase("Tzaangors", 6, WOUNDS, 5, 5, false),
    m_savageBlade(Weapon::Type::Melee, "Savage Blade", 1, 2, 4, 4, 0, 1),
    m_savageBladeTwistbray(Weapon::Type::Melee, "Savage Blade", 1, 2, 3, 4, 0, 1),
    m_savageGreatblade(Weapon::Type::Melee, "Savage Greatblade", 1, 1, 4, 4, -1, 2),
    m_savageGreatbladeTwistbray(Weapon::Type::Melee, "Savage Greatblade", 1, 1, 3, 4, -1, 2),
    m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
    m_viciousBeakTwistbray(Weapon::Type::Melee, "Vicious Beak", 1, 1, 3, 5, 0, 1)
{
    m_keywords = {CHAOS, GOR, TZEENTCH, ARCANITE, TZAANGORS};
    m_weapons = {&m_savageBlade, &m_savageBladeTwistbray, &m_savageGreatblade, &m_savageGreatbladeTwistbray, &m_viciousBeak, &m_viciousBeakTwistbray};
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

    m_weaponOption = weapons;
    m_numGreatblades = numGreatblades;
    m_numMutants = numMutants;
    m_iconBearer = iconBearer;
    m_brayhorns = brayhorns;

    // Brayhorns
    m_runAndCharge = brayhorns;

    auto twistbray = new Model(BASESIZE, wounds());
    if (numGreatblades)
        twistbray->addMeleeWeapon(&m_savageGreatbladeTwistbray);
    else
        twistbray->addMeleeWeapon(&m_savageBladeTwistbray);
    twistbray->addMeleeWeapon(&m_viciousBeakTwistbray);
    addModel(twistbray);

    // the Twistbray is always given a greatblade if there are any greatblades
    for (auto i = 1; i < numGreatblades; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_savageGreatblade);
        model->addMeleeWeapon(&m_viciousBeak);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_savageBlade);
        model->addMeleeWeapon(&m_viciousBeak);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *Tzaangors::Create(const ParameterList &parameters)
{
    auto unit = new Tzaangors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOptions)GetEnumParam("Weapons", parameters, SavageBladeAndShield);
    int numGreatblades = GetIntParam("Greatblades", parameters, 0);
    int numMutants = GetIntParam("Mutants", parameters, 0);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool brayhorns = GetBoolParam("Brayhorns", parameters, false);

    auto coven = (ChangeCoven)GetEnumParam("Change Coven", parameters, TzeentchBase::None);
    unit->setChangeCoven(coven);

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
        static FactoryMethod factoryMethod = {
            Tzaangors::Create,
            Tzaangors::ValueToString,
            Tzaangors::EnumStringToInt,
            Tzaangors::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", Tzaangors::MIN_UNIT_SIZE, Tzaangors::MIN_UNIT_SIZE,
                    Tzaangors::MAX_UNIT_SIZE, Tzaangors::MIN_UNIT_SIZE
                },
                {
                    ParamType::Enum, "Weapons", Tzaangors::PairedSavageBlades, Tzaangors::PairedSavageBlades,
                    Tzaangors::SavageBladeAndShield, 1
                },
                {ParamType::Integer, "Greatblades", 0, 0, Tzaangors::MAX_UNIT_SIZE/Tzaangors::MIN_UNIT_SIZE*2, 1},
                {ParamType::Integer, "Mutants", 0, 0, Tzaangors::MAX_UNIT_SIZE/Tzaangors::MIN_UNIT_SIZE, 1},
                {ParamType::Boolean, "Icon Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, 1},
                {ParamType::Boolean, "Brayhorns", SIM_FALSE, SIM_FALSE, SIM_FALSE, 1},
                {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None, TzeentchBase::GuildOfSummoners, 1},
            },
            CHAOS,
            { TZEENTCH, BEASTS_OF_CHAOS }
        };
        s_registered = UnitFactory::Register("Tzaangors", factoryMethod);
    }
}

std::string Tzaangors::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == SavageBladeAndShield)
        {
            return "Savage Blade And Shield";
        }
        else if (parameter.intValue == PairedSavageBlades)
        {
            return "Paired Savage Blades";
        }
    }

    return TzeentchBase::ValueToString(parameter);
}

int Tzaangors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Savage Blade And Shield")
    {
        return SavageBladeAndShield;
    }
    else if (enumString == "Paired Savage Blades")
    {
        return PairedSavageBlades;
    }
    return TzeentchBase::EnumStringToInt(enumString);
}

Wounds Tzaangors::applyWoundSave(const Wounds &wounds)
{
    // Arcanite Shield
    if (m_weaponOption == SavageBladeAndShield)
    {
        auto roll = Dice::rollD6();
        if (roll == 6)
            return {0, 0};
    }
    return TzeentchBase::applyWoundSave(wounds);
}

int Tzaangors::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = TzeentchBase::toHitModifier(weapon, target);

    // Paired Savage Blades
    if ((m_weaponOption == PairedSavageBlades) && (weapon->name() == m_savageBlade.name()))
        modifier += 1;

    return modifier;
}

int Tzaangors::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = TzeentchBase::toWoundModifier(weapon, target);

    // Destined Mayhem
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0f);
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

int Tzaangors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = TzeentchBase::extraAttacks(nullptr, weapon, target);

    // Savagery Unleashed
    if (remainingModels() >= 9) attacks++;

    return attacks;
}

int Tzaangors::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Tzeentch