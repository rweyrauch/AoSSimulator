/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/BoarboyManiaks.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageBoarboyManiaks::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "Models", SavageBoarboyManiaks::MIN_UNIT_SIZE, SavageBoarboyManiaks::MIN_UNIT_SIZE, SavageBoarboyManiaks::MAX_UNIT_SIZE, SavageBoarboyManiaks::MIN_UNIT_SIZE},
        {ParamType::Boolean, "Boar Thumper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Bone Totem Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageBoarboyManiaks::s_registered = false;

Unit *SavageBoarboyManiaks::Create(const ParameterList &parameters)
{
    auto unit = new SavageBoarboyManiaks();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool thumper = GetBoolParam("Boar Thumper", parameters, true);
    bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);

    bool ok = unit->configure(numModels, thumper, totem);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SavageBoarboyManiaks::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Boarboy Maniaks", factoryMethod);
    }
}

SavageBoarboyManiaks::SavageBoarboyManiaks() :
    Unit("Savage Boarboy Maniaks", 12, WOUNDS, 6, 6, false),
    m_chompas(Weapon::Type::Melee, "Pair of Chompas", 1, 4, 4, 3, 0, 1),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1),
    m_chompasBoss(Weapon::Type::Melee, "Pair of Chompas", 1, 5, 4, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, BOARBOYS, SAVAGE_BOARBOY_MANIAKS};
}

bool SavageBoarboyManiaks::configure(int numModels, bool boarThumper, bool totemBearer)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_thumper = boarThumper;
    m_totemBearer = totemBearer;

    // Add the Boss
    Model bossModel(BASESIZE, WOUNDS);
    bossModel.addMeleeWeapon(&m_chompasBoss);
    bossModel.addMeleeWeapon(&m_tusksAndHooves);
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_chompas);
        model.addMeleeWeapon(&m_tusksAndHooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

int SavageBoarboyManiaks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Maniak Fury
    if (weapon->name() == m_chompas.name() && remainingModels() >= 5)
    {
        return 1;
    }
    return Unit::extraAttacks(attackingModel, weapon, target);
}

int SavageBoarboyManiaks::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    // Tusker Charge
    auto mod = Unit::toHitModifier(weapon, target);
    if (m_charged && weapon->name() == m_tusksAndHooves.name())
    {
        mod++;
    }
    return mod;
}

int SavageBoarboyManiaks::toWoundModifier(const Weapon *weapon, const Unit *target) const
{
    // Tusker Charge
    auto mod = Unit::toWoundModifier(weapon, target);
    if (m_charged && weapon->name() == m_tusksAndHooves.name())
    {
        mod++;
    }
    return mod;
}

void SavageBoarboyManiaks::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_chompas);
    visitor(&m_chompasBoss);
    visitor(&m_tusksAndHooves);
}

} // namespace Bonesplitterz
