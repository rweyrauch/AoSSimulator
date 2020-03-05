/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "slavestodarkness/OgroidMyrmidon.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    OgroidMyrmidon::Create,
    SlavesToDarknessBase::ValueToString,
    SlavesToDarknessBase::EnumStringToInt,
    OgroidMyrmidon::ComputePoints,
    {
        {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
    },
    CHAOS,
    { SLAVES_TO_DARKNESS }
};

bool OgroidMyrmidon::s_registered = false;

Unit *OgroidMyrmidon::Create(const ParameterList &parameters)
{
    auto unit = new OgroidMyrmidon();

    auto legion = (DamnedLegion)GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
    unit->setDamnedLegion(legion);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void OgroidMyrmidon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ogroid Myrmidon", factoryMethod);
    }
}

OgroidMyrmidon::OgroidMyrmidon() :
    SlavesToDarknessBase("Ogroid Myrmidon", 6, WOUNDS, 8, 4, false),
    m_spearMissile(Weapon::Type::Missile, "Gladiator Spear", 18, 1, 3, 3, -1, RAND_D3),
    m_spear(Weapon::Type::Melee, "Gladiator Spear", 2, 6, 3, 3, -1, 1),
    m_horns(Weapon::Type::Melee, "Great Horns", 1, 1, 3, 3, -2, 3)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, HERO, EYE_OF_THE_GODS, OGROID_MYRMIDON};
    m_weapons = {&m_spearMissile, &m_spear, &m_horns};
}

bool OgroidMyrmidon::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);

    model->addMissileWeapon(&m_spearMissile);
    model->addMeleeWeapon(&m_spear);
    model->addMeleeWeapon(&m_horns);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int OgroidMyrmidon::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Arcane Fury
    if ((unmodifiedHitRoll == 6) && (!weapon->isMissile()))
    {
        return 2;
    }
    return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
}

}//namespace SlavesToDarkness

