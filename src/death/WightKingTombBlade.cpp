/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/WightKingTombBlade.h>
#include <UnitFactory.h>

namespace Death
{
static FactoryMethod factoryMethod = {
    WightKingWithBalefulTombBlade::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Boolean, "Steed", {.m_boolValue = true}, false, false},
    },
    DEATH,
    DEATHRATTLE
};

bool WightKingWithBalefulTombBlade::s_registered = false;

WightKingWithBalefulTombBlade::WightKingWithBalefulTombBlade() :
    Unit("Wight King with Baleful Tomb Blade", 4, WOUNDS, 10, 3, false),
    m_balefulTombBlade(Weapon::Type::Melee, "Baleful Tomb Blade", 1, 4, 3, 3, -1, 1),
    m_steedsHoovesAndTeeth(Weapon::Type::Melee, "Skeletal Steed's Hooves and Teeth", 1, 2, 4, 5, 0, 1)
{
    m_keywords = {DEATH, SKELETON, DEATHRATTLE, HERO, WIGHT_KING};
}

void WightKingWithBalefulTombBlade::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_balefulTombBlade);
    visitor(&m_steedsHoovesAndTeeth);
}

Unit *WightKingWithBalefulTombBlade::Create(const ParameterList &parameters)
{
    auto unit = new WightKingWithBalefulTombBlade();
    bool steed = GetBoolParam("Steed", parameters, false);

    bool ok = unit->configure(steed);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WightKingWithBalefulTombBlade::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Wight King with Baleful Tomb Blade", factoryMethod);
    }
}

bool WightKingWithBalefulTombBlade::configure(bool hasSteed)
{
    Model model(BASESIZE, WOUNDS);

    m_hasSteed = hasSteed;

    model.addMeleeWeapon(&m_balefulTombBlade);
    if (m_hasSteed)
    {
        model.addMeleeWeapon(&m_steedsHoovesAndTeeth);
        m_move = 12;
    }
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Wounds WightKingWithBalefulTombBlade::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Beheading Strike
    if ((hitRoll >= 6) && (weapon->name() == m_balefulTombBlade.name()))
    {
        Dice dice;
        return {dice.rollD3(), 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace Death
