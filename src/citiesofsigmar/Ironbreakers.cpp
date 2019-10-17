/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Ironbreakers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace CitiesOfSigmar
{

static FactoryMethod factoryMethod = {
    Ironbreakers::Create,
    Ironbreakers::ValueToString,
    Ironbreakers::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Ironbreakers::MIN_UNIT_SIZE, Ironbreakers::MIN_UNIT_SIZE,
            Ironbreakers::MAX_UNIT_SIZE, Ironbreakers::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Ironbeard Weapon", Ironbreakers::IronbreakerAxeOrHammer, Ironbreakers::IronbreakerAxeOrHammer,
            Ironbreakers::PairedDrakefirePistols, 1
        },
        {ParamType::Boolean, "Icon Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Drummer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
    },
    ORDER,
    DISPOSSESSED
};

bool Ironbreakers::s_registered = false;

Ironbreakers::Ironbreakers() :
    CitizenOfSigmar("Ironbreakers", 4, WOUNDS, 7, 4, false),
    m_drakefirePistol(Weapon::Type::Missile, "Drakefire Pistol", 8, 1, 4, 3, -1, 1),
    m_drakefirePistolMelee(Weapon::Type::Melee, "Drakefire Pistol", 1, 1, 4, 4, 0, 1),
    m_axeOrHammer(Weapon::Type::Melee, "Ironbreaker Axe or Hammer", 1, 2, 3, 4, 0, 1),
    m_axeOrHammerIronbeard(Weapon::Type::Melee, "Ironbreaker Axe or Hammer", 1, 3, 3, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, IRONBREAKERS};
}

bool Ironbreakers::configure(int numModels, WeaponOptions ironbeardWeapons, bool iconBearer, bool drummer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_drummer = drummer;

    Model ironbeard(BASESIZE, WOUNDS);
    if (ironbeardWeapons == IronbreakerAxeOrHammer)
    {
        ironbeard.addMeleeWeapon(&m_axeOrHammerIronbeard);
    }
    else if (ironbeardWeapons == DrakefirePistolAndCinderblastBomb)
    {
        ironbeard.addMissileWeapon(&m_drakefirePistol);
        ironbeard.addMeleeWeapon(&m_drakefirePistolMelee);
        m_hasCinderblastBomb = true;
    }
    else if (ironbeardWeapons == PairedDrakefirePistols)
    {
        // two attacks when using dual-pistols
        ironbeard.addMissileWeapon(&m_drakefirePistol);
        ironbeard.addMissileWeapon(&m_drakefirePistol);
        ironbeard.addMeleeWeapon(&m_drakefirePistolMelee);
        ironbeard.addMeleeWeapon(&m_drakefirePistolMelee);
    }
    addModel(ironbeard);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_axeOrHammer);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Ironbreakers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_drakefirePistol);
    visitor(&m_drakefirePistolMelee);
    visitor(&m_axeOrHammer);
    visitor(&m_axeOrHammerIronbeard);
}

Unit *Ironbreakers::Create(const ParameterList &parameters)
{
    auto unit = new Ironbreakers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapon = (WeaponOptions)GetEnumParam("Ironbeard Weapon", parameters, (int)IronbreakerAxeOrHammer);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool drummer = GetBoolParam("Drummer", parameters, false);

    bool ok = unit->configure(numModels, weapon, iconBearer, drummer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Ironbreakers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ironbreakers", factoryMethod);
    }
}

Rerolls Ironbreakers::toSaveRerolls(const Weapon *weapon) const
{
    // Gromril Shields
    if (!m_ran && !m_charged)
    {
        if (!weapon->isMissile())
            return RerollFailed;
    }
    return CitizenOfSigmar::toSaveRerolls(weapon);
}

int Ironbreakers::toSaveModifier(const Weapon *weapon) const
{
    int modifier = CitizenOfSigmar::toSaveModifier(weapon);

    // Forge-proven Gromril Armour - ignore rend of less than -2 by cancelling it out.
    if (weapon->rend() == -1)
    {
        modifier = -weapon->rend();
    }

    return modifier;
}

void Ironbreakers::onStartShooting(PlayerId player)
{
    CitizenOfSigmar::onStartShooting(player);

    // Cinderblast Bomb
    if (m_hasCinderblastBomb)
    {
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 6);
        if (!units.empty())
        {
            Dice dice;
            int roll = dice.rollD6();
            if (roll >= 2)
            {
                units.front()->applyDamage({0, dice.rollD3()});
            }
            m_hasCinderblastBomb = false;
        }
    }
}

std::string Ironbreakers::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Ironbeard Weapon")
    {
        if (parameter.m_intValue == IronbreakerAxeOrHammer)
        {
            return "Ironbreaker Axe Or Hammer";
        }
        else if (parameter.m_intValue == DrakefirePistolAndCinderblastBomb)
        {
            return "Drakefire Pistol And Cinderblast Bomb";
        }
        else if (parameter.m_intValue == PairedDrakefirePistols)
        {
            return "Paired Drakefire Pistols";
        }
    }

    return CitizenOfSigmar::ValueToString(parameter);
}

int Ironbreakers::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Ironbreaker Axe Or Hammer")
    {
        return IronbreakerAxeOrHammer;
    }
    else if (enumString == "Drakefire Pistol And Cinderblast Bomb")
    {
        return DrakefirePistolAndCinderblastBomb;
    }
    else if (enumString == "Paired Drakefire Pistols")
    {
        return PairedDrakefirePistols;
    }
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

int Ironbreakers::rollRunDistance() const
{
    // Sound the Advance
    if (m_drummer)
    {
        return 4;
    }
    return Unit::rollRunDistance();
}

} // namespace CitiesOfSigmar