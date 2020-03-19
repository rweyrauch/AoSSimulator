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

struct TableEntry
{
    int m_move;
    int m_hornDamage;
    int m_stompAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 9, Stegadon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, 4, 5},
        {7,  3,  4},
        {6,  2,  3},
        {5,  2,  2},
        {4,  1,  1}
    };

bool Stegadon::s_registered = false;

Stegadon::Stegadon() :
    SeraphonBase("Stegadon", 8, WOUNDS, 5, 4, false),
    m_javelins(Weapon::Type::Missile, "Meteoric Javelins", 8, 4, 5, 4, 0, 1),
    m_bow(Weapon::Type::Missile, "Skystreak Bow", 24, 3, 3, 3, -1, 3),
    m_throwers(Weapon::Type::Missile, "Sunfire Throwers", 8, 1, 0, 0, 0, 0),
    m_warspear(Weapon::Type::Melee, "Meteoric Warspear", 1, 3, 3, 3, -1, 1),
    m_horns(Weapon::Type::Melee, "Massive Horns", 2, 2, 3, 3, -1, 4),
    m_jaws(Weapon::Type::Melee, "Grinding Jaws", 1, 2, 3, 3, -1, 2),
    m_stomps(Weapon::Type::Melee, "Crushing Stomps", 1, 5, 3, 3, -1, 2)
{
    m_keywords = {ORDER, SERAPHON, SKINK, MONSTER, STEGADON};
    m_weapons = {&m_javelins, &m_bow, &m_throwers, &m_warspear, &m_horns, &m_jaws, &m_stomps};

    s_globalBattleshockReroll.connect(this, &Stegadon::steadfastMajestyBraveryReroll, &m_steadfastSlot);
}

Stegadon::~Stegadon()
{
    m_steadfastSlot.disconnect();
}

bool Stegadon::configure(WeaponOption option, bool skinkChief)
{
    m_skinkChief = skinkChief;

    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_javelins);
    if (option == SkystreakBow)
        model->addMissileWeapon(&m_bow);
    else if (option == SunfireThrowers)
        model->addMissileWeapon(&m_throwers);
    model->addMeleeWeapon(&m_horns);
    model->addMeleeWeapon(&m_jaws);
    model->addMeleeWeapon(&m_stomps);

    if (m_skinkChief)
    {
        model->addMeleeWeapon(&m_warspear);
        addKeyword(HERO);
    }
    addModel(model);

    m_points = POINTS_PER_UNIT;
    if (m_skinkChief) m_points = POINTS_PER_UNIT_WITH_CHIEF;

    return true;
}

void Stegadon::onRestore()
{
    // Reset table-drive attributes
    onWounded();
}

Unit *Stegadon::Create(const ParameterList &parameters)
{
    auto unit = new Stegadon();
    auto option = (WeaponOption)GetEnumParam("Weapon", parameters, SkystreakBow);
    bool chief = GetBoolParam("Skink Chief", parameters, false);

    auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
    auto constellation = (Constellation)GetEnumParam("Constellation", parameters, SeraphonBase::None);
    unit->setWayOfTheSeraphon(way, constellation);

    bool ok = unit->configure(option, chief);
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
        static FactoryMethod factoryMethod = {
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Weapon", SkystreakBow, SkystreakBow, SunfireThrowers, 1},
                {ParamType::Boolean, "Skink Chief", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne, SeraphonBase::Coalesced, 1},
                {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None, SeraphonBase::FangsOfSotek, 1}
            },
            ORDER,
            { SERAPHON }
        };

        s_registered = UnitFactory::Register("Stegadon", factoryMethod);
    }
}

std::string Stegadon::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == SkystreakBow) return "Skystreak Bow";
        else if (parameter.intValue == SunfireThrowers) return "Sunfire Throwers";
    }
    return SeraphonBase::ValueToString(parameter);
}

void Stegadon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_stomps.setAttacks(g_damageTable[damageIndex].m_stompAttacks);
    m_horns.setDamage(g_damageTable[damageIndex].m_hornDamage);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
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
    if (enumString == "Skystreak Bow") return SkystreakBow;
    else if (enumString == "Sunfire Throwers") return SunfireThrowers;

    return SeraphonBase::EnumStringToInt(enumString);
}

void Stegadon::onCharged()
{
    SeraphonBase::onCharged();

    // Unstoppable Stampede
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
    for (auto unit : units)
    {
        if (Dice::rollD6() >= 3)
        {
            unit->applyDamage({0, Dice::rollD3()});
        }
    }
}

Rerolls Stegadon::steadfastMajestyBraveryReroll(const Unit *unit)
{
    if (isFriendly(unit) && unit->hasKeyword(SKINK) && (distanceTo(unit) <= 18.0f)) return RerollFailed;
    return NoRerolls;
}

} //namespace Seraphon
