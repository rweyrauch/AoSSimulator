/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Loonboss.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    Loonboss::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool Loonboss::s_registered = false;

Loonboss::Loonboss() :
    GloomspiteGitzBase("Loonboss", 5, WOUNDS, 5, 5, false),
    m_moonslicer(Weapon::Type::Melee, "Moon-slicer", 1, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, LOONBOSS};
}

bool Loonboss::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_moonslicer);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Loonboss::Create(const ParameterList &parameters)
{
    auto unit = new Loonboss();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Loonboss::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Loonboss", factoryMethod);
    }
}

void Loonboss::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_moonslicer);
}

int Loonboss::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
    // Dead Tricksy
    modifier -= 1;
    return modifier;
}

} // namespace GloomspiteGitz
