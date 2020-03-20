/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/OrrukArrowBoys.h"

namespace Bonesplitterz
{
static const int BASESIZE = 32;
static const int WOUNDS = 2;
static const int MIN_UNIT_SIZE = 10;
static const int MAX_UNIT_SIZE = 30;
static const int POINTS_PER_BLOCK = 120;
static const int POINTS_MAX_UNIT_SIZE = 360;

bool SavageOrrukArrowboys::s_registered = false;

Unit *SavageOrrukArrowboys::Create(const ParameterList &parameters)
{
    auto unit = new SavageOrrukArrowboys();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool thumper = GetBoolParam("Skull Thumper", parameters, true);
    bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Bonesplitterz::Bonegrinz);
    unit->setWarclan(warclan);

    bool ok = unit->configure(numModels, thumper, totem);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SavageOrrukArrowboys::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            Bonesplitterz::ValueToString,
            Bonesplitterz::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Boolean, "Skull Thumper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Boolean, "Bone Totem Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                {ParamType::Enum, "Warclan", Bonesplitterz::Bonegrinz, Bonesplitterz::Bonegrinz, Bonesplitterz::Icebone, 1},
            },
            DESTRUCTION,
            { BONESPLITTERZ }
        };
        s_registered = UnitFactory::Register("Savage Orruk Arrowboys", factoryMethod);
    }
}

SavageOrrukArrowboys::SavageOrrukArrowboys() :
    Bonesplitterz("Savage Orruk Arrowboys", 5, WOUNDS, 5, 6, false),
    m_stingaBow(Weapon::Type::Missile, "Stinga Bow", 18, 2, 5, 4, 0, 1),
    m_boneShiv(Weapon::Type::Melee, "Bone Shiv", 1, 1, 4, 4, 0, 1),
    m_chompa(Weapon::Type::Melee, "Chompa", 1, 3, 4, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUK_ARROWBOYS};
    m_weapons = {&m_stingaBow, &m_boneShiv, &m_chompa};
}

bool SavageOrrukArrowboys::configure(int numModels, bool skullThumper, bool totemBearer)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_thumper = skullThumper;
    m_totemBearer = totemBearer;

    // Add the Boss
    auto bossModel = new Model(BASESIZE, wounds());
    bossModel->addMissileWeapon(&m_stingaBow);
    bossModel->addMeleeWeapon(&m_chompa);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_stingaBow);
        model->addMeleeWeapon(&m_boneShiv);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int SavageOrrukArrowboys::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extra = Unit::extraAttacks(attackingModel, weapon, target);
    // Loadsa Arrows
    if (weapon->name() == m_stingaBow.name() && remainingModels() >= 15)
    {
        extra++;
    }
    return extra;
}

int SavageOrrukArrowboys::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Aim Fer Its Eyes
    if (weapon->name() == m_stingaBow.name() && target->hasKeyword(MONSTER))
    {
        return -1;
    }
    return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
}

int SavageOrrukArrowboys::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Bonesplitterz
