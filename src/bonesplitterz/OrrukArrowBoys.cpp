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
static FactoryMethod factoryMethod = {
    SavageOrrukArrowboys::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "Models", SavageOrrukArrowboys::MIN_UNIT_SIZE, SavageOrrukArrowboys::MIN_UNIT_SIZE, SavageOrrukArrowboys::MAX_UNIT_SIZE, SavageOrrukArrowboys::MIN_UNIT_SIZE},
        {ParamType::Boolean, "Skull Thumper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Bone Totem Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageOrrukArrowboys::s_registered = false;

Unit *SavageOrrukArrowboys::Create(const ParameterList &parameters)
{
    auto unit = new SavageOrrukArrowboys();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool thumper = GetBoolParam("Skull Thumper", parameters, true);
    bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);

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
        s_registered = UnitFactory::Register("Savage Orruk Arrowboys", factoryMethod);
    }
}

SavageOrrukArrowboys::SavageOrrukArrowboys() :
    Unit("Savage Orruk Arrowboys", 5, WOUNDS, 5, 6, false),
    m_stingaBow(Weapon::Type::Missile, "Stinga Bow", 18, 2, 5, 4, 0, 1),
    m_boneShiv(Weapon::Type::Melee, "Bone Shiv", 1, 1, 4, 4, 0, 1),
    m_chompa(Weapon::Type::Melee, "Chompa", 1, 3, 4, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUK_ARROWBOYS};
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
    Model bossModel(BASESIZE, WOUNDS);
    bossModel.addMissileWeapon(&m_stingaBow);
    bossModel.addMeleeWeapon(&m_chompa);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_stingaBow);
        model.addMeleeWeapon(&m_boneShiv);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SavageOrrukArrowboys::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_stingaBow);
    visitor(&m_boneShiv);
    visitor(&m_chompa);
}

int SavageOrrukArrowboys::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Loadsa Arrows
    if (weapon->name() == m_stingaBow.name() && remainingModels() >= 15)
    {
        return 1;
    }
    return Unit::extraAttacks(attackingModel, weapon, target);
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

} // namespace Bonesplitterz
