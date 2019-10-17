/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Doomflayer.h>
#include <UnitFactory.h>

namespace Skaven
{
static FactoryMethod factoryMethod = {
    Doomflayer::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SKAVEN
};

bool Doomflayer::s_registered = false;

Doomflayer::Doomflayer() :
    Skaventide("Doom-flayer", RAND_2D6, WOUNDS, 4, 5, false),
    m_whirlingBlades(Weapon::Type::Melee, "Whirling Blades", 1, RAND_D6, 3, 3, -1, 1),
    m_rustyKnives(Weapon::Type::Melee, "Rusty Knives", 1, 2, 5, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, WEAPON_TEAM, DOOM_FLAYER};
}

bool Doomflayer::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_whirlingBlades);
    model.addMeleeWeapon(&m_rustyKnives);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Doomflayer::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_whirlingBlades);
    visitor(m_rustyKnives);
}

Unit *Doomflayer::Create(const ParameterList &parameters)
{
    auto unit = new Doomflayer();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Doomflayer::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Doom-flayer", factoryMethod);
    }
}

int Doomflayer::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Skaventide::toHitModifier(weapon, target);

    // Whirling Death
    if (m_charged && (weapon->name() == m_whirlingBlades.name()))
    {
        modifier += 1;
    }

    return modifier;
}


} // namespace Skaven
