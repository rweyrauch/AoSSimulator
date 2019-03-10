/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/GryphHounds.h>
#include <UnitFactory.h>
#include <Board.h>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    GryphHounds::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = GryphHounds::MIN_UNIT_SIZE}, GryphHounds::MIN_UNIT_SIZE,
            GryphHounds::MAX_UNIT_SIZE, GryphHounds::MIN_UNIT_SIZE
        },
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool GryphHounds::s_registered = false;

GryphHounds::GryphHounds() :
    StormcastEternal("Gryph-hounds", 9, WOUNDS, 6, NoSave, false),
    m_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 3, 4, 0, 1),
    m_beakAndClawsAlpha(Weapon::Type::Melee, "Beak and Claws (Alpha)", 1, 3, 3, 4, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, STORMCAST_ETERNAL, GRYPH_HOUNDS};
}

bool GryphHounds::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model alpha(BASESIZE, WOUNDS);
    alpha.addMeleeWeapon(&m_beakAndClawsAlpha);
    addModel(alpha);

    for (int i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_beakAndClaws);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void GryphHounds::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_beakAndClaws);
    visitor(&m_beakAndClawsAlpha);
}

Unit *GryphHounds::Create(const ParameterList &parameters)
{
    auto unit = new GryphHounds();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GryphHounds::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Gryph-hounds", factoryMethod);
    }
}

int GryphHounds::extraAttacks(const Weapon *weapon) const
{
    int attacks = StormcastEternal::extraAttacks(weapon);

    // Loyal Companion
    auto units = Board::Instance()->getUnitsWithin(this, m_owningPlayer, 6);
    for (auto ip : units)
    {
        if (ip->hasKeyword(LORD_CASTELLANT) || (ip->hasKeyword(LORD_VERITANT)))
        {
            attacks += 2;
        }
    }
    return attacks;
}

} //namespace StormcastEternals