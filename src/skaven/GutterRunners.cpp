/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GutterRunners.h>
#include <UnitFactory.h>

namespace Skaven
{
static const int BASESIZE = 25;
static const int WOUNDS = 1;
static const int MIN_UNIT_SIZE = 5;
static const int MAX_UNIT_SIZE = 20;
static const int POINTS_PER_BLOCK = 60;
static const int POINTS_MAX_UNIT_SIZE = 200;

bool GutterRunners::s_registered = false;

GutterRunners::GutterRunners() :
    Skaventide("Gutter Runners", 7, WOUNDS, 5, 5, false),
    m_throwingStars(Weapon::Type::Missile, "Eshin Throwing Stars", 12, 2, 4, 5, 0, 1),
    m_punchDaggerAndBlade(Weapon::Type::Melee, "Punch Daggar and Blade", 1, 2, 3, 4, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_ESHIN, GUTTER_RUNNERS};
    m_weapons = {&m_throwingStars, &m_punchDaggerAndBlade};
}

bool GutterRunners::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    // Running Death
    m_runAndShoot = true;

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_throwingStars);
        model->addMeleeWeapon(&m_punchDaggerAndBlade);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *GutterRunners::Create(const ParameterList &parameters)
{
    auto unit = new GutterRunners();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void GutterRunners::Init()
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
        s_registered = UnitFactory::Register("Gutter Runners", factoryMethod);
    }
}

int GutterRunners::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Throwing Stars
    if ((unmodifiedHitRoll == 6) && (weapon->name() == m_throwingStars.name()))
    {
        return weapon->attacks()+1;
    }
    return Skaventide::generateHits(unmodifiedHitRoll, weapon, unit);
}

int GutterRunners::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;

}

} // namespace Skaven
