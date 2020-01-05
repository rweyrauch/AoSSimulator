/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Stormfiends.h>
#include <UnitFactory.h>

namespace Skaven
{

static FactoryMethod factoryMethod = {
    Stormfiends::Create,
    Stormfiends::ValueToString,
    Stormfiends::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Stormfiends::MIN_UNIT_SIZE, Stormfiends::MIN_UNIT_SIZE,
            Stormfiends::MAX_UNIT_SIZE, Stormfiends::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapon A", Stormfiends::WarpfireProjectors, Stormfiends::WarpfireProjectors,
            Stormfiends::Windlaunchers, 1
        },
        {
            ParamType::Enum, "Weapon B", Stormfiends::Grinderfists, Stormfiends::Grinderfists,
            Stormfiends::RatlingCannons, 1
        },
        {
            ParamType::Enum, "Weapon C", Stormfiends::DoomflayerGauntlets, Stormfiends::DoomflayerGauntlets,
            Stormfiends::ShockGauntlets, 1
        },
    },
    CHAOS,
    SKAVEN
};

bool Stormfiends::s_registered = false;

Stormfiends::Stormfiends() :
    Skaventide("Stormfiends", 6, WOUNDS, 6, 4, false),
    m_ratlingCannons(Weapon::Type::Missile, "Ratling Cannons", 12, RAND_3D6, 4, 3, -1, 1),
    m_windlaunchers(Weapon::Type::Missile, "Windlaunchers", 24, 3, 4, 4, -3, RAND_D3),
    m_warpfireProjectors(Weapon::Type::Missile, "Warpfire Projectors", 8, 0, 0, 0, 0, 0),
    m_doomfireGauntlets(Weapon::Type::Melee, "Doomflayer Gauntlets", 1, RAND_2D3, 3, 3, -2, RAND_D3),
    m_grinderfists(Weapon::Type::Melee, "Grinderfists", 1, 4, 4, 3, -2, 2),
    m_shockGauntlets(Weapon::Type::Melee, "Shock Gauntlets", 1, 4, 4, 3, -1, 2),
    m_clubbingBlows(Weapon::Type::Melee, "Clubbing Blows", 1, 4, 4, 3, 0, 2)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_MOULDER, CLANS_SKRYRE, STORMFIENDS};
    m_weapons = {&m_ratlingCannons, &m_windlaunchers, &m_warpfireProjectors, &m_doomfireGauntlets, &m_grinderfists, &m_shockGauntlets, &m_clubbingBlows};
}

bool Stormfiends::configure(int numModels, Stormfiends::WeaponOption_1 weapon1, Stormfiends::WeaponOption_2 weapon2, Stormfiends::WeaponOption_3 weapon3)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weapons1 = weapon1;
    m_weapons2 = weapon2;
    m_weapons3 = weapon3;

    int numWeapon1 = numModels / 3;

    for (auto i = 0; i < numWeapon1; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapon1 == WarpfireProjectors)
        {
            model->addMissileWeapon(&m_warpfireProjectors);
        }
        else if (weapon1 == Windlaunchers)
        {
            model->addMissileWeapon(&m_windlaunchers);
        }
        model->addMeleeWeapon(&m_clubbingBlows);
        addModel(model);
    }

    int numWeapon2 = numModels / 3;
    for (auto i = 0; i < numWeapon2; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapon2 == Grinderfists)
        {
            model->addMeleeWeapon(&m_grinderfists);
        }
        else if (weapon2 == RatlingCannons)
        {
            model->addMissileWeapon(&m_ratlingCannons);
            model->addMeleeWeapon(&m_clubbingBlows);
        }
        addModel(model);
    }

    int numWeapon3 = numModels - (numWeapon1 + numWeapon2);
    for (auto i = 0; i < numWeapon3; i++)
    {
        // Warpstone-laced Armour => +1 wounds
        auto model = new Model(BASESIZE, WOUNDS+1);
        if (weapon3 == DoomflayerGauntlets)
        {
            model->addMeleeWeapon(&m_doomfireGauntlets);
        }
        else if (weapon3 == ShockGauntlets)
        {
            model->addMeleeWeapon(&m_shockGauntlets);
        }
        addModel(model);
    }

    return true;
}

Unit *Stormfiends::Create(const ParameterList &parameters)
{
    auto unit = new Stormfiends();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption_1 weapon1 = (WeaponOption_1)GetEnumParam("Weapon A", parameters, WarpfireProjectors);
    WeaponOption_2 weapon2 = (WeaponOption_2)GetEnumParam("Weapon B", parameters, Grinderfists);
    WeaponOption_3 weapon3 = (WeaponOption_3)GetEnumParam("Weapon C", parameters, DoomflayerGauntlets);

    bool ok = unit->configure(numModels, weapon1, weapon2, weapon3);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Stormfiends::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Stormfiends", factoryMethod);
    }
}

std::string Stormfiends::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon A")
    {
        if (parameter.m_intValue == WarpfireProjectors)
        {
            return "Warpfire Projectors";
        }
        else if (parameter.m_intValue == Windlaunchers)
        {
            return "Windlaunchers";
        }
    }
    else if (parameter.m_name == "Weapon B")
    {
        if (parameter.m_intValue == Grinderfists)
        {
            return "Grinderfists";
        }
        else if (parameter.m_intValue == RatlingCannons)
        {
            return "Ratling Cannons";
        }
    }
    else if (parameter.m_name == "Weapon C")
    {
        if (parameter.m_intValue == DoomflayerGauntlets)
        {
            return "Doomflayer Gauntlets";
        }
        else if (parameter.m_intValue == ShockGauntlets)
        {
            return "Shock Gauntlets";
        }
    }

    return ParameterValueToString(parameter);
}

int Stormfiends::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Warpfire Projectors")
    {
        return WarpfireProjectors;
    }
    else if (enumString == "Windlaunchers")
    {
        return Windlaunchers;
    }
    else if (enumString == "Grinderfists")
    {
        return Grinderfists;
    }
    else if (enumString == "Ratling Cannons")
    {
        return RatlingCannons;
    }
    else if (enumString == "Doomflayer Gauntlets")
    {
        return DoomflayerGauntlets;
    }
    else if (enumString == "Shock Gauntlets")
    {
        return ShockGauntlets;
    }
    return 0;
}

} // namespace Skaven
