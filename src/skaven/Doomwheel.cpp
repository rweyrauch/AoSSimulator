/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Doomwheel.h>
#include <UnitFactory.h>

namespace Skaven
{
static FactoryMethod factoryMethod = {
    Doomwheel::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SKAVEN
};

bool Doomwheel::s_registered = false;

Doomwheel::Doomwheel() :
    Skaventide("Doomwheel", RAND_4D6, WOUNDS, 7, 4, false),
    m_warpBolts(Weapon::Type::Missile, "Warp Bolts", 13, RAND_D6, 3, 3, -1, RAND_D3),
    m_grindingWheel(Weapon::Type::Melee, "Grinding Wheel", 1, RAND_D6, 3, 3, -1, 1),
    m_teethAndKnives(Weapon::Type::Melee, "Teeth and Knives", 1, 6, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WAR_MACHINE, DOOMWHEEL};
}

bool Doomwheel::configure()
{
        auto model = new Model(BASESIZE, WOUNDS);

    model->addMissileWeapon(&m_warpBolts);
    model->addMeleeWeapon(&m_grindingWheel);
    model->addMeleeWeapon(&m_teethAndKnives);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Doomwheel::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_warpBolts);
    visitor(m_grindingWheel);
    visitor(m_teethAndKnives);
}

Unit *Doomwheel::Create(const ParameterList &parameters)
{
    auto unit = new Doomwheel();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Doomwheel::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Doomwheel", factoryMethod);
    }
}


} // namespace Skaven
