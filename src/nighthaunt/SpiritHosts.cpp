/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/SpiritHosts.h>
#include <UnitFactory.h>

namespace Nighthaunt
{
static FactoryMethod factoryMethod = {
    SpiritHosts::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", SpiritHosts::MIN_UNIT_SIZE, SpiritHosts::MIN_UNIT_SIZE,
            SpiritHosts::MAX_UNIT_SIZE, SpiritHosts::MIN_UNIT_SIZE
        },
    },
    DEATH,
    NIGHTHAUNT
};

bool SpiritHosts::s_registered = false;

SpiritHosts::SpiritHosts() :
    Nighthaunt("Spirit Hosts", 6, WOUNDS, 10, 4, true),
    m_spectralClawsAndDaggars(Weapon::Type::Melee, "Spectral Claws and Daggers", 1, 6, 5, 4, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, SPIRIT_HOSTS};
    m_weapons = {&m_spectralClawsAndDaggars};
}

bool SpiritHosts::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_spectralClawsAndDaggars);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *SpiritHosts::Create(const ParameterList &parameters)
{
    auto unit = new SpiritHosts();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SpiritHosts::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Spirit Hosts", factoryMethod);
    }
}

Wounds SpiritHosts::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Frightful Touch
    if ((woundRoll == 6) && (weapon->name() == m_spectralClawsAndDaggars.name()))
    {
        return {0, 1};
    }
    return Nighthaunt::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} //namespace Nighthaunt
