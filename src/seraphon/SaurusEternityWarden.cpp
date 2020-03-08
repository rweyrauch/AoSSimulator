/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusEternityWarden.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon
{
static FactoryMethod factoryMethod = {
    SaurusEternityWarden::Create,
    SeraphonBase::ValueToString,
    SeraphonBase::EnumStringToInt,
    SaurusEternityWarden::ComputePoints,
    {},
    ORDER,
    { SERAPHON }
};

bool SaurusEternityWarden::s_registered = false;

SaurusEternityWarden::SaurusEternityWarden() :
    SeraphonBase("Saurus Eternity Warden", 5, WOUNDS, 8, 3, false),
    m_mace(Weapon::Type::Melee, "Star-stone Mace", 1, 3, 3, 3, -1, 1),
    m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1)
{
    m_keywords = {ORDER, SERAPHON, SAURUS, HERO, ETERNITY_WARDEN};
    m_weapons = {&m_mace, &m_jaws};
}

bool SaurusEternityWarden::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_mace);
    model->addMeleeWeapon(&m_jaws);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

Unit *SaurusEternityWarden::Create(const ParameterList &parameters)
{
    auto unit = new SaurusEternityWarden();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SaurusEternityWarden::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Saurus Eternity Warden", factoryMethod);
    }
}

int SaurusEternityWarden::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Cold Ferocity
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_mace.name()))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

} //namespace Seraphon
