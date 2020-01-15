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
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    GryphHounds::ComputePoints,
    {
        {
            ParamType::Integer, "Models", GryphHounds::MIN_UNIT_SIZE, GryphHounds::MIN_UNIT_SIZE,
            GryphHounds::MAX_UNIT_SIZE, GryphHounds::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    { STORMCAST_ETERNAL }
};

bool GryphHounds::s_registered = false;

GryphHounds::GryphHounds() :
    StormcastEternal("Gryph-hounds", 9, WOUNDS, 6, NoSave, false),
    m_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 3, 4, 0, 1),
    m_beakAndClawsAlpha(Weapon::Type::Melee, "Beak and Claws", 1, 3, 3, 4, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, STORMCAST_ETERNAL, GRYPH_HOUNDS};
    m_weapons = {&m_beakAndClaws, &m_beakAndClawsAlpha};
}

bool GryphHounds::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    auto alpha = new Model(BASESIZE, WOUNDS);
    alpha->addMeleeWeapon(&m_beakAndClawsAlpha);
    addModel(alpha);

    for (int i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_beakAndClaws);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *GryphHounds::Create(const ParameterList &parameters)
{
    auto unit = new GryphHounds();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

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

int GryphHounds::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = StormcastEternal::extraAttacks(nullptr, weapon, target);

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

int GryphHounds::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace StormcastEternals