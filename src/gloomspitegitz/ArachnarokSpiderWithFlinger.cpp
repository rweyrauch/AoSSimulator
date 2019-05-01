/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/ArachnarokSpiderWithFlinger.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    ArachnarokSpiderWithFlinger::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool ArachnarokSpiderWithFlinger::s_registered = false;

struct TableEntry
{
    int m_move;
    int m_legsAttacks;
    int m_fangsToHit;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, ArachnarokSpiderWithFlinger::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {8, 8, 2},
        {7, 7, 3},
        {6, 6, 3},
        {5, 5, 4},
        {4, 4, 4}
    };

ArachnarokSpiderWithFlinger::ArachnarokSpiderWithFlinger() :
    GloomspiteGitzBase("Arachnarok Spider with Flinger", 8, WOUNDS, 6, 4, true),
    m_spiderBows(Weapon::Type::Missile, "Spider-bows", 16, 8, 5, 5, 0, 1),
    m_flinger(Weapon::Type::Missile, "Flinger", 36, 1, 0, 0, 0, 0),
    m_chitinousLegs(Weapon::Type::Melee, "Chitinous Legs", 3, 8, 4, 3, -1, 1),
    m_monstrousFangs(Weapon::Type::Melee, "Monstrous Fangs", 1, 4, 2, 3, -1, RAND_D3),
    m_crookedSpears(Weapon::Type::Melee, "Crooked Spears", 1, 8, 5, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ARACHNAROK_SPIDER, GLOOMSPITE_GITZ, SPIDERFANG, MONSTER};
}

bool ArachnarokSpiderWithFlinger::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_spiderBows);
    model.addMissileWeapon(&m_flinger);
    model.addMeleeWeapon(&m_chitinousLegs);
    model.addMeleeWeapon(&m_monstrousFangs);
    model.addMeleeWeapon(&m_crookedSpears);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int ArachnarokSpiderWithFlinger::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void ArachnarokSpiderWithFlinger::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_monstrousFangs.setToHit(g_damageTable[damageIndex].m_fangsToHit);
    m_chitinousLegs.setAttacks(g_damageTable[damageIndex].m_legsAttacks);
}

int ArachnarokSpiderWithFlinger::getDamageTableIndex() const
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

Unit *ArachnarokSpiderWithFlinger::Create(const ParameterList &parameters)
{
    auto unit = new ArachnarokSpiderWithFlinger();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ArachnarokSpiderWithFlinger::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Arachnarok Spider with Flinger", factoryMethod);
    }
}

void ArachnarokSpiderWithFlinger::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_spiderBows);
    visitor(&m_flinger);
    visitor(&m_chitinousLegs);
    visitor(&m_monstrousFangs);
    visitor(&m_crookedSpears);
}

} // namespace GloomspiteGitz