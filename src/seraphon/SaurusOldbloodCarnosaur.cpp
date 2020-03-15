/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusOldbloodCarnosaur.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{

struct TableEntry
{
    int m_move;
    int m_forelimbToHit;
    int m_jawsDamage;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, SaurusOldbloodOnCarnosaur::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {10, 3, 5},
        {9,  4,  4},
        {8,  4,  3},
        {7,  5,  2},
        {6,  5,  1}
    };

bool SaurusOldbloodOnCarnosaur::s_registered = false;

SaurusOldbloodOnCarnosaur::SaurusOldbloodOnCarnosaur() :
    SeraphonBase("Saurus Oldblood on Carnosaur", 10, WOUNDS, 8, 4, false),
    m_gauntlet(Weapon::Type::Missile, "Sunbolt Gauntlet", 18, RAND_D6, 3, 4, -1, 1),
    m_spear(Weapon::Type::Melee, "Sunstone Spear", 2, 3, 3, 3, -1, 3),
    m_forelimbs(Weapon::Type::Melee, "Clawed Forelimbs", 2, 2,3, 3, 0, 2),
    m_jaws(Weapon::Type::Melee, "Massive Jaws", 2, 3, 4, 3, -1, 5)
{
    m_keywords = {ORDER, SERAPHON, CARNOSAUR, SAURUS, MONSTER, HERO, OLDBLOOD};
    m_weapons = {&m_gauntlet, &m_spear, &m_forelimbs, &m_jaws};

    s_globalBraveryMod.connect(this, &SaurusOldbloodOnCarnosaur::terror, &m_connection);
}

SaurusOldbloodOnCarnosaur::~SaurusOldbloodOnCarnosaur()
{
    m_connection.disconnect();
}

bool SaurusOldbloodOnCarnosaur::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_gauntlet);
    model->addMeleeWeapon(&m_spear);
    model->addMeleeWeapon(&m_forelimbs);
    model->addMeleeWeapon(&m_jaws);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

void SaurusOldbloodOnCarnosaur::onRestore()
{
    // Reset table-drive attributes
    onWounded();
}

Unit *SaurusOldbloodOnCarnosaur::Create(const ParameterList &parameters)
{
    auto unit = new SaurusOldbloodOnCarnosaur();

    auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
    auto constellation = (Constellation)GetEnumParam("Constellation", parameters, SeraphonBase::None);
    unit->setWayOfTheSeraphon(way, constellation);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SaurusOldbloodOnCarnosaur::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            SaurusOldbloodOnCarnosaur::Create,
            SeraphonBase::ValueToString,
            SeraphonBase::EnumStringToInt,
            SaurusOldbloodOnCarnosaur::ComputePoints,
            {
                {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne, SeraphonBase::Coalesced, 1},
                {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None, SeraphonBase::FangsOfSotek, 1}
            },
            ORDER,
            { SERAPHON }
        };

        s_registered = UnitFactory::Register("Saurus Oldblood on Carnosaur", *factoryMethod);
    }
}

void SaurusOldbloodOnCarnosaur::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_jaws.setDamage(g_damageTable[damageIndex].m_jawsDamage);
    m_forelimbs.setToHit(g_damageTable[damageIndex].m_forelimbToHit);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

int SaurusOldbloodOnCarnosaur::getDamageTableIndex() const
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

int SaurusOldbloodOnCarnosaur::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Cold Ferocity
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_spear.name()))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

int SaurusOldbloodOnCarnosaur::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Pinned Down
    if ((weapon->name() == m_jaws.name()) && (target->wounds() >= 7))
    {
        mod++;
    }

    return mod;
}

int SaurusOldbloodOnCarnosaur::terror(const Unit *target)
{
    // Terror
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }
    return 0;
}

} //namespace Seraphon
