/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/GreatBrayShaman.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{

static FactoryMethod factoryMethod = {
    GreatBrayShaman::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool GreatBrayShaman::s_registered = false;

GreatBrayShaman::GreatBrayShaman() :
    BeastsOfChaosBase("Great Bray-shaman", 6, WOUNDS, 6, 6, false),
    m_fetishStaff(Weapon::Type::Melee, "Fetish Staff", 2, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, HERO, WIZARD, GREAT_BRAY_SHAMAN};
}

bool GreatBrayShaman::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_fetishStaff);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *GreatBrayShaman::Create(const ParameterList &parameters)
{
    auto unit = new GreatBrayShaman();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GreatBrayShaman::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Great Bray-shaman", factoryMethod);
    }
}

void GreatBrayShaman::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_fetishStaff);
}

} // namespace BeastsOfChaos
