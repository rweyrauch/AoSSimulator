/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/SteamTank.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    SteamTank::Create,
    SteamTank::ValueToString,
    SteamTank::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

struct TableEntry
{
    int m_move;
    int m_canonRange;
    int m_gunToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, SteamTank::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {RAND_2D6, 30, 2},
        {RAND_2D6, 24, 3},
        {RAND_D6, 18, 4},
        {RAND_D6,  12, 5},
        {RAND_D3,  6, 6}
    };

bool SteamTank::s_registered = false;

Unit *SteamTank::Create(const ParameterList &parameters)
{
    auto unit = new SteamTank();

    auto commander = GetBoolParam("Commander", parameters, true);

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure(commander);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string SteamTank::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int SteamTank::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void SteamTank::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Steam Tank", factoryMethod);
    }
}

SteamTank::SteamTank() :
    CitizenOfSigmar("Steam Tank", RAND_2D6, WOUNDS, 8, 3, false),
    m_steamCannon(Weapon::Type::Missile, "Steam Cannon", 30, 1, 4, 2, -2, RAND_D6),
    m_steamGun(Weapon::Type::Missile, "Steam Gun", 8, RAND_2D6, 4, 2, 0, 1),
    m_longRifle(Weapon::Type::Missile, "Long Rifle", 30, 1, 3, 3, -1, 2),
    m_handgun(Weapon::Type::Missile, "Repeater Handgun", 16, RAND_D3, 4, 3, -1, 1),
    m_crushingWheels(Weapon::Type::Melee, "Crushing Wheels", 1, RAND_D6, 4, 3, -1, 2),
    m_sword(Weapon::Type::Melee, "Sword or Rod", 1, 2, 5, 4, 0, 1)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, WAR_MACHINE, STEAM_TANK};
}

bool SteamTank::configure(bool commander)
{

        auto model = new Model(BASESIZE, WOUNDS);

    model->addMissileWeapon(&m_steamCannon);
    model->addMissileWeapon(&m_steamGun);
    model->addMeleeWeapon(&m_crushingWheels);

    m_points = POINTS_PER_UNIT;

    if (commander)
    {
        addKeyword(HERO);
        model->addMissileWeapon(&m_longRifle);
        model->addMissileWeapon(&m_handgun);
        model->addMeleeWeapon(&m_sword);

        m_points += POINTS_FOR_COMMANDER;
    }

    addModel(model);

    return true;
}

void SteamTank::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_steamCannon);
    visitor(m_steamGun);
    visitor(m_longRifle);
    visitor(m_handgun);
    visitor(m_crushingWheels);
    visitor(m_sword);
}

int SteamTank::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void SteamTank::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_steamCannon.setRange(g_damageTable[damageIndex].m_canonRange);
    m_steamGun.setToWound(g_damageTable[damageIndex].m_gunToWound);

    // TODO: change range of model's Steam Cannon

    Unit::onWounded();
}

int SteamTank::getDamageTableIndex() const
{
    auto woundsInflicted = wounds() - remainingWounds();
    for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++)
    {
        if (woundsInflicted < g_woundThresholds[i])
        {
            return i;
        }
    }
    return 0;
}

} // namespace CitiesOfSigmar