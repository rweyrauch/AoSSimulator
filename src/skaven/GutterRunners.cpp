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

static FactoryMethod factoryMethod = {
    GutterRunners::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", GutterRunners::MIN_UNIT_SIZE, GutterRunners::MIN_UNIT_SIZE,
            GutterRunners::MAX_UNIT_SIZE, GutterRunners::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    { SKAVEN }
};

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
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_throwingStars);
        model->addMeleeWeapon(&m_punchDaggerAndBlade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

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

} // namespace Skaven
