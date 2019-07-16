/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Branchwraith.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    Branchwraith::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    SYLVANETH
};

bool Branchwraith::s_registered = false;

Branchwraith::Branchwraith() :
    SylvanethBase("Branchwraith", 7, WOUNDS, 8, 5, false),
    m_piercingTalons(Weapon::Type::Melee, "Piercing Talons", 2, 3, 4, 4, -1, 1)
{
    m_keywords = {ORDER, SYLVANETH, FOREST_FOLK, HERO, WIZARD, BRANCHWRAITH};
}

bool Branchwraith::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_piercingTalons);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Branchwraith::Create(const ParameterList &parameters)
{
    auto unit = new Branchwraith();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Branchwraith::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Branchwraith", factoryMethod);
    }
}

void Branchwraith::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_piercingTalons);
}

int Branchwraith::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
{
    int modifier = Unit::targetHitModifier(weapon, attacker);

    // Blessing of the Forest
    auto unit = Board::Instance()->getUnitWithKeyword(this, m_owningPlayer, AWAKENED_WYLDWOOD, 6.0f);
    if (unit != nullptr)
    {
        modifier -= 1;
    }

    return modifier;
}

} // namespace Sylvaneth
