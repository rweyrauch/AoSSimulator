/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Stegadon.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    Stegadon::Create,
    Stegadon::ValueToString,
    Stegadon::EnumStringToInt,
    Stegadon::ComputePoints,
    {
        {ParamType::Enum, "Weapon", Stegadon::SkystreakBow, Stegadon::SkystreakBow, Stegadon::SunfireThrowers, 1},
    },
    ORDER,
    { SERAPHON }
};

struct TableEntry
{
    int m_move;
    int m_hornRend;
    int m_stompAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, Stegadon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, -3, RAND_3D6},
        {7,  -2,  RAND_2D6},
        {6,  -2,  RAND_2D6},
        {5,  -1,  RAND_D6},
        {4,  -1,  RAND_D6}
    };

bool Stegadon::s_registered = false;

Stegadon::Stegadon() :
    SeraphonBase("Stegadon", 8, WOUNDS, 10, 4, false),
    m_javelins(Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1),
    m_bow(Weapon::Type::Missile, "Skystreak Bow", 25, 3, 4, 3, -1, RAND_D3),
    m_throwers(Weapon::Type::Missile, "Sunfire Throwers", 8, 1, 3, 3, 0, 1),
    m_horns(Weapon::Type::Melee, "Massive Horns", 2, 3, 3, 3, -3, 2),
    m_stomps(Weapon::Type::Melee, "Crushing Stomps", 1, RAND_3D6, 4, 3, 0, 1)
{
    m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SKINK, MONSTER, STEGADON};
    m_weapons = {&m_javelins, &m_bow, &m_throwers, &m_horns, &m_stomps};
}

bool Stegadon::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_javelins);
    if (option == SkystreakBow)
        model->addMissileWeapon(&m_bow);
    else if (option == SunfireThrowers)
        model->addMissileWeapon(&m_throwers);
    model->addMeleeWeapon(&m_horns);
    model->addMeleeWeapon(&m_stomps);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

int Stegadon::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *Stegadon::Create(const ParameterList &parameters)
{
    auto unit = new Stegadon();
    auto option = (WeaponOption)GetEnumParam("Weapon", parameters, SkystreakBow);

    bool ok = unit->configure(option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Stegadon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Stegadon", factoryMethod);
    }
}

std::string Stegadon::ValueToString(const Parameter &parameter)
{
    return SeraphonBase::ValueToString(parameter);
}

void Stegadon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_stomps.setAttacks(g_damageTable[damageIndex].m_stompAttacks);
    m_horns.setRend(g_damageTable[damageIndex].m_hornRend);
}

int Stegadon::getDamageTableIndex() const
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

int Stegadon::EnumStringToInt(const std::string &enumString)
{
    return SeraphonBase::EnumStringToInt(enumString);
}

} //namespace Seraphon
