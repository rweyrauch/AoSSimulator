/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/BattlemageOnGriffon.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    BattlemageOnGriffon::Create,
    BattlemageOnGriffon::ValueToString,
    BattlemageOnGriffon::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

struct TableEntry
{
    int m_move;
    int m_beaksDamage;
    int m_clawAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 11, BattlemageOnGriffon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {15, 3, 6},
        {13, 2, 5},
        {11, 2, 4},
        {9,  1, 3},
        {7,  1, 2}
    };

bool BattlemageOnGriffon::s_registered = false;

Unit *BattlemageOnGriffon::Create(const ParameterList &parameters)
{
    auto unit = new BattlemageOnGriffon();

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

std::string BattlemageOnGriffon::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int BattlemageOnGriffon::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void BattlemageOnGriffon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Battlemage on Griffon", factoryMethod);
    }
}

BattlemageOnGriffon::BattlemageOnGriffon() :
    CitizenOfSigmar("Battlemage on Griffon", 15, WOUNDS, 6, 5, true),
    m_beastStaff(Weapon::Type::Melee, "Beaststaff", 2, 1, 4, 3, -1, RAND_D3),
    m_twinBeaks(Weapon::Type::Melee, "Twin Beaks", 2, 4, 3, 3, -1, 3),
    m_razorClaws(Weapon::Type::Melee, "Razor Claws", 2, 6, 4, 3, -1, 2)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, MONSTER, HERO, WIZARD, BATTLEMAGE};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool BattlemageOnGriffon::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_beastStaff);
    model.addMeleeWeapon(&m_twinBeaks);
    model.addMeleeWeapon(&m_razorClaws);

    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void BattlemageOnGriffon::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_beastStaff);
    visitor(m_twinBeaks);
    visitor(m_razorClaws);
}

int BattlemageOnGriffon::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void BattlemageOnGriffon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_twinBeaks.setDamage(g_damageTable[damageIndex].m_beaksDamage);
    m_razorClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);

    // TODO: set beak damage.

    Unit::onWounded();
}

int BattlemageOnGriffon::getDamageTableIndex() const
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

int BattlemageOnGriffon::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Two-headed
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_twinBeaks.name()))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

} //namespace CitiesOfSigmar