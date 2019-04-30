/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Beastlord.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{

static FactoryMethod factoryMethod = {
    Beastlord::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool Beastlord::s_registered = false;

Beastlord::Beastlord() :
    BeastsOfChaosBase("Beastlord", 6, WOUNDS, 7, 4, false),
    m_pairedAxes(Weapon::Type::Melee, "Paired Man-ripper Axes", 1, 6, 3, 3, -1, 1)
{
    m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, HERO, BEASTLORD};
}

bool Beastlord::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_pairedAxes);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Beastlord::Create(const ParameterList &parameters)
{
    auto unit = new Beastlord();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Beastlord::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Beastlord", factoryMethod);
    }
}

void Beastlord::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_pairedAxes);
}

} // namespace BeastsOfChaos
