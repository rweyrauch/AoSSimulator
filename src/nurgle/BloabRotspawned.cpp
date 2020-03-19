/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nurgle/BloabRotspawned.h"

namespace Nurgle
{
static FactoryMethod factoryMethod = {
    BloabRotspawned::Create,
    NurgleBase::ValueToString,
    NurgleBase::EnumStringToInt,
    BloabRotspawned::ComputePoints,
    {
    },
    CHAOS,
    { NURGLE }
};

bool BloabRotspawned::s_registered = false;

Unit* BloabRotspawned::Create(const ParameterList &parameters)
{
    auto unit = new BloabRotspawned();
    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BloabRotspawned::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloab Rotspawned", factoryMethod);
    }
}

BloabRotspawned::BloabRotspawned() :
    NurgleBase("Bloab Rotspawned", 10, WOUNDS, 9, 4, false),
    m_bile(Weapon::Type::Missile, "Bilespurter's Vile Bile", 12, RAND_D3, 4, 2, -2, RAND_D3),
    m_scythe(Weapon::Type::Melee, "Harvestman's Scythe", 2, 3, 3,3, -1, 2),
    m_claws(Weapon::Type::Melee, "Bilespurter's Monstrous Claws", 3, 5, 4, 2, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, MONSTER, HERO, WIZARD, BLOAB_ROTSPAWNED};
    m_weapons = {&m_bile, &m_scythe, &m_claws};

    m_totalUnbinds = 1;
    m_totalSpells = 1;

    s_globalCastMod.connect(this, &BloabRotspawned::windspeakerBellsCastingMod, &m_windspeakerSlot);
}

BloabRotspawned::~BloabRotspawned()
{
    m_windspeakerSlot.disconnect();
}

bool BloabRotspawned::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_bile);
    model->addMeleeWeapon(&m_scythe);
    model->addMeleeWeapon(&m_claws);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

int BloabRotspawned::windspeakerBellsCastingMod(const Unit *caster)
{
    // Windspeaker Bells
    if (!isFriendly(caster) && caster->hasKeyword(WIZARD) && (distanceTo(caster) <= 14.0f))
    {
        return -1;
    }
    return 0;
}

} // namespace Nurgle