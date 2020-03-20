/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Packmasters.h>
#include <UnitFactory.h>

namespace Skaven
{
static const int BASESIZE = 25;
static const int WOUNDS = 2;
static const int MIN_UNIT_SIZE = 3;
static const int MAX_UNIT_SIZE = 12;
static const int POINTS_PER_BLOCK = 60;
static const int POINTS_MAX_UNIT_SIZE = 240;

bool Packmasters::s_registered = false;

Unit *Packmasters::Create(const ParameterList &parameters)
{
    auto unit = new Packmasters();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numCatchers = GetIntParam("Thing-catchers", parameters, MIN_UNIT_SIZE/3);

    bool ok = unit->configure(numModels, numCatchers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int Packmasters::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

void Packmasters::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE,MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Integer, "Thing-catchers", MIN_UNIT_SIZE/3, MIN_UNIT_SIZE/3, MAX_UNIT_SIZE/3, MIN_UNIT_SIZE/3},
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Packmasters", factoryMethod);
    }
}

Packmasters::Packmasters() :
    Skaventide("Packmasters", 6, WOUNDS, 5, 6, false),
    m_whip(Weapon::Type::Melee, "Herding Whip", 3, 1, 4, 4, 0, 1),
    m_blade(Weapon::Type::Melee, "Rusty Blade", 1, 2, 4, 4, 0, 1),
    m_catcher(Weapon::Type::Melee, "Things-catcher", 2, 1, 4, 4, -1, 2)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_MOULDER, PACKMASTERS};
    m_weapons = {&m_whip, &m_blade, &m_catcher};

    s_globalToHitMod.connect(this, &Packmasters::crackTheWhip, &m_whipSlot);
    s_globalBraveryMod.connect(this, &Packmasters::crackTheWhipBravery, &m_whipBraverySlot);
}

Packmasters::~Packmasters()
{
    m_whipSlot.disconnect();
    m_whipBraverySlot.disconnect();
}

bool Packmasters::configure(int numModels, int numCatchers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    const int maxCatchers = numModels/3;
    if (numCatchers > maxCatchers)
    {
        return false;
    }

    for (auto i = 0; i < numCatchers; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_whip);
        model->addMeleeWeapon(&m_catcher);
        addModel(model);
    }
    for (auto i = numCatchers; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_whip);
        model->addMeleeWeapon(&m_blade);
        addModel(model);
    }
    m_points = ComputePoints(numModels);

    return true;
}

int Packmasters::crackTheWhip(const Unit *attacker, const Weapon *weapon, const Unit *target)
{
    // Crack the Whip
    if (attacker->hasKeyword(CLANS_MOULDER) && attacker->hasKeyword(PACK) && (distanceTo(attacker) <= 12.0f))
    {
        return 1;
    }
    return 0;
}

int Packmasters::crackTheWhipBravery(const Unit *unit)
{
    // Crack the Whip
    if (unit->hasKeyword(CLANS_MOULDER) && unit->hasKeyword(PACK) && (distanceTo(unit) <= 12.0f))
    {
        // Double unit's bravery
        return unit->bravery();
    }
    return 0;
}

} //namespace Skaven
