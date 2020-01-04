/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/NightRunners.h>
#include <UnitFactory.h>

namespace Skaven
{
static FactoryMethod factoryMethod = {
    NightRunners::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", NightRunners::MIN_UNIT_SIZE, NightRunners::MIN_UNIT_SIZE,
            NightRunners::MAX_UNIT_SIZE, NightRunners::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    SKAVEN
};

bool NightRunners::s_registered = false;

NightRunners::NightRunners() :
    Skaventide("Night Runners", 7, WOUNDS, 4, 6, false),
    m_throwingWeapons(Weapon::Type::Missile, "Eshin Throwing Weapons", 12, 1, 4, 5, 0, 1),
    m_stabbingBlade(Weapon::Type::Melee, "Stabbing Blade", 1, 1, 4, 4, 0, 1),
    m_stabbingBladeLeader(Weapon::Type::Melee, "Stabbing Blade", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_ESHIN, NIGHT_RUNNERS};
}

bool NightRunners::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    // Running Death
    m_runAndShoot = true;

    auto leader = new Model(BASESIZE, WOUNDS);
    leader->addMissileWeapon(&m_throwingWeapons);
    leader->addMeleeWeapon(&m_stabbingBladeLeader);
    addModel(leader);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_throwingWeapons);
        model->addMeleeWeapon(&m_stabbingBlade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void NightRunners::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_throwingWeapons);
    visitor(m_stabbingBlade);
    visitor(m_stabbingBladeLeader);
}

Unit *NightRunners::Create(const ParameterList &parameters)
{
    auto unit = new NightRunners();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void NightRunners::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Night Runners", factoryMethod);
    }
}

int NightRunners::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Throwing Weapons
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_throwingWeapons.name()))
    {
        return weapon->attacks()+1;
    }
    return Skaventide::generateHits(unmodifiedHitRoll, weapon, unit);
}

} // namespace Skaven
