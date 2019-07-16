/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Dryads.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace Sylvaneth
{

static FactoryMethod factoryMethod = {
    Dryads::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", Dryads::MIN_UNIT_SIZE, Dryads::MIN_UNIT_SIZE,
            Dryads::MAX_UNIT_SIZE, Dryads::MIN_UNIT_SIZE
        },
    },
    ORDER,
    SYLVANETH
};

bool Dryads::s_registered = false;

Dryads::Dryads() :
    SylvanethBase("Dryads", 7, WOUNDS, 6, 5, false),
    m_wrackingTalons(Weapon::Type::Melee, "Wracking Talons", 2, 2, 4, 4, 0, 1),
    m_wrackingTalonsNymph(Weapon::Type::Melee, "Wracking Talons (Nymph)", 2, 3, 4, 4, 0, 1)
{
    m_keywords = {ORDER, SYLVANETH, FOREST_FOLK, DRYADS};
}

bool Dryads::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model nymph(BASESIZE, WOUNDS);
    nymph.addMeleeWeapon(&m_wrackingTalonsNymph);
    addModel(nymph);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_wrackingTalons);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *Dryads::Create(const ParameterList &parameters)
{
    auto unit = new Dryads();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Dryads::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dryads", factoryMethod);
    }
}

void Dryads::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_wrackingTalons);
    visitor(&m_wrackingTalonsNymph);
}

int Dryads::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    // Impenetrable Thicket
    if (remainingModels() >= 10) modifier += 1;

    return modifier;
}

int Dryads::targetHitModifier(const Weapon *weapon, const Unit *attacker) const
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