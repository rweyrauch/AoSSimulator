/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Necromancer.h>
#include <UnitFactory.h>

namespace Death
{
static FactoryMethod factoryMethod = {
    Necromancer::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    DEATHMAGES
};

bool Necromancer::s_registered = false;

Necromancer::Necromancer() :
    Unit("Necromancer", 5, WOUNDS, 10, 6, false),
    m_staff(Weapon::Type::Melee, "Necromancer's Staff", 2, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {DEATH, NECROMANCER, DEATHMAGES, HERO, WIZARD};
}

bool Necromancer::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_staff);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Necromancer::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_staff);
}

Unit *Necromancer::Create(const ParameterList &parameters)
{
    auto unit = new Necromancer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Necromancer::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Necromancer", factoryMethod);
    }
}

Wounds Necromancer::applyWoundSave(const Wounds &wounds)
{
    return Unit::applyWoundSave(wounds);
}

} // namespace Death
