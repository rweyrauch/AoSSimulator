/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/OrrukMorboys.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageOrrukMorboys::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "Models", SavageOrrukMorboys::MIN_UNIT_SIZE, SavageOrrukMorboys::MIN_UNIT_SIZE, SavageOrrukMorboys::MAX_UNIT_SIZE, SavageOrrukMorboys::MIN_UNIT_SIZE},
        {ParamType::Boolean, "Skull Thumper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Bone Totem Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageOrrukMorboys::s_registered = false;

Unit *SavageOrrukMorboys::Create(const ParameterList &parameters)
{
    auto unit = new SavageOrrukMorboys();
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

void SavageOrrukMorboys::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Orruk Morboys", factoryMethod);
    }
}

SavageOrrukMorboys::SavageOrrukMorboys() :
    Unit("Savage Orruk Morboys", 5, WOUNDS, 6, 6, false),
    m_chompaAndShiv(Weapon::Type::Melee, "Chompa and Toof Shiv", 1, 3, 4, 3, 0, 1),
    m_chompaAndShivBoss(Weapon::Type::Melee, "Chompa and Toof Shiv", 1, 4, 4, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUK_MORBOYS};
}

bool SavageOrrukMorboys::configure(int numModels, bool skullThumper, bool totemBearer)
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
    bossModel.addMeleeWeapon(&m_chompaAndShivBoss);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_chompaAndShiv);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SavageOrrukMorboys::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_chompaAndShiv);
    visitor(&m_chompaAndShivBoss);
}

int SavageOrrukMorboys::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Spirit of Gorkamorka
    if (remainingModels() >= 15)
    {
        return 1;
    }
    return Unit::extraAttacks(attackingModel, weapon, target);
}

} // namespace Bonesplitterz
