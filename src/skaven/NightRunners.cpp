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
static const int BASESIZE = 25;
static const int WOUNDS = 1;
static const int MIN_UNIT_SIZE = 10;
static const int MAX_UNIT_SIZE = 40;
static const int POINTS_PER_BLOCK = 80;
static const int POINTS_MAX_UNIT_SIZE = 280;

bool NightRunners::s_registered = false;

NightRunners::NightRunners() :
    Skaventide("Night Runners", 7, WOUNDS, 4, 6, false),
    m_throwingWeapons(Weapon::Type::Missile, "Eshin Throwing Weapons", 12, 1, 4, 5, 0, 1),
    m_stabbingBlade(Weapon::Type::Melee, "Stabbing Blade", 1, 1, 4, 4, 0, 1),
    m_stabbingBladeLeader(Weapon::Type::Melee, "Stabbing Blade", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_ESHIN, NIGHT_RUNNERS};
    m_weapons = {&m_throwingWeapons, &m_stabbingBlade, &m_stabbingBladeLeader};
}

bool NightRunners::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    // Running Death
    m_runAndShoot = true;

    auto leader = new Model(BASESIZE, wounds());
    leader->addMissileWeapon(&m_throwingWeapons);
    leader->addMeleeWeapon(&m_stabbingBladeLeader);
    addModel(leader);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_throwingWeapons);
        model->addMeleeWeapon(&m_stabbingBlade);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
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
        static FactoryMethod factoryMethod = {
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
            },
            CHAOS,
            { SKAVEN }
        };
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

int NightRunners::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} // namespace Skaven
