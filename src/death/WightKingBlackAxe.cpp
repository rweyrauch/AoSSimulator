/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/WightKingBlackAxe.h>
#include <UnitFactory.h>

namespace Death
{
static FactoryMethod factoryMethod = {
    WightKingWithBlackAxe::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    DEATHRATTLE
};

bool WightKingWithBlackAxe::s_registered = false;

WightKingWithBlackAxe::WightKingWithBlackAxe() :
    Unit("Wight King with Black Axe", 4, WOUNDS, 10, 4, false),
    m_blackAxe(Weapon::Type::Melee, "Black Axe", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {DEATH, SKELETON, DEATHRATTLE, HERO, WIGHT_KING};
    m_weapons = {&m_blackAxe};
}

bool WightKingWithBlackAxe::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_blackAxe);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *WightKingWithBlackAxe::Create(const ParameterList &parameters)
{
    auto unit = new WightKingWithBlackAxe();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WightKingWithBlackAxe::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Wight King with Black Axe", factoryMethod);
    }
}

Wounds WightKingWithBlackAxe::applyWoundSave(const Wounds &wounds)
{
    // Barrow Armour
    return {(wounds.normal+1)/2, wounds.mortal};
}

} // namespace Death
