/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Branchwych.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    Branchwych::Create,
    SylvanethBase::ValueToString,
    SylvanethBase::EnumStringToInt,
    {
        {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None, SylvanethBase::Harvestboon, 1},
    },
    ORDER,
    SYLVANETH
};

bool Branchwych::s_registered = false;

Branchwych::Branchwych() :
    SylvanethBase("Branchwych", 7, WOUNDS, 7, 5, false),
    m_greenwoodScythe(Weapon::Type::Melee, "Greenwood Scythe", 2, 2, 4, 3, 0, 2),
    m_bittergrubsMandibles(Weapon::Type::Melee, "Snapping Mandibles", 1, 1, 4, 4, -1, 1)
{
    m_keywords = {ORDER, SYLVANETH, NOBLE_SPIRITS, HERO, WIZARD, BRANCHWYCH};
    m_weapons = {&m_greenwoodScythe, &m_bittergrubsMandibles};
}

bool Branchwych::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_greenwoodScythe);
    model->addMeleeWeapon(&m_bittergrubsMandibles);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Branchwych::Create(const ParameterList &parameters)
{
    auto unit = new Branchwych();

    auto glade = (Glade)GetEnumParam("Glade", parameters, SylvanethBase::None);
    unit->setGlade(glade);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Branchwych::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Branchwych", factoryMethod);
    }
}

int Branchwych::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Quick-tempered
    if ((weapon->name() == m_greenwoodScythe.name()) && (attackingModel->woundsRemaining() < wounds()))
    {
        return 2;
    }
    return Unit::extraAttacks(attackingModel, weapon, target);
}

int Branchwych::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);

    // Fury of the Forest
    auto unit = Board::Instance()->getUnitWithKeyword(this, m_owningPlayer, AWAKENED_WYLDWOOD, 6.0f);
    if (unit != nullptr)
    {
        modifier += 1;
    }

    return modifier;
}

} // namespace Sylvaneth
