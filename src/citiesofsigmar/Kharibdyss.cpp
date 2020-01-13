/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Kharibdyss.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    Kharibdyss::Create,
    Kharibdyss::ValueToString,
    Kharibdyss::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    { CITIES_OF_SIGMAR }
};

bool Kharibdyss::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_tentacleAttacks;
    int m_tailToWound;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {1, 3, 5, 7, Kharibdyss::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {7, 6, 2},
        {6, 5, 3},
        {5, 4, 4},
        {5,  3, 5},
        {4,  2, 6}
    };

Unit *Kharibdyss::Create(const ParameterList &parameters)
{
    auto unit = new Kharibdyss();

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Kharibdyss::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int Kharibdyss::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void Kharibdyss::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Kharibdyss", factoryMethod);
    }
}

Kharibdyss::Kharibdyss() :
    CitizenOfSigmar("Kharibdyss", 7, WOUNDS, 6, 4, false),
    m_tentacles(Weapon::Type::Melee, "Fanged Tentacles", 3, 6, 4, 3, -1, 2),
    m_tail(Weapon::Type::Melee, "Spiked Tail", 2, RAND_D6, 4, 2, 0, 1),
    m_limbs(Weapon::Type::Melee, "Clawed Limbs", 1, 2, 3, 3, -1, 1),
    m_goadsAndWhips(Weapon::Type::Melee, "Cruel Goads and Whips", 2, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, MONSTER, KHARIBDYSS};
    m_weapons = {&m_tentacles, &m_tail, &m_limbs, &m_goadsAndWhips};
}

bool Kharibdyss::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_tentacles);
    model->addMeleeWeapon(&m_tail);
    model->addMeleeWeapon(&m_limbs);
    model->addMeleeWeapon(&m_goadsAndWhips);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int Kharibdyss::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void Kharibdyss::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_tentacles.setAttacks(g_damageTable[damageIndex].m_tentacleAttacks);
    m_tail.setToWound(g_damageTable[damageIndex].m_tailToWound);

    Unit::onWounded();
}

int Kharibdyss::getDamageTableIndex() const
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