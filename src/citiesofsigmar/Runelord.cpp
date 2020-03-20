/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Runelord.h>
#include <UnitFactory.h>

namespace CitiesOfSigmar
{
static const int BASESIZE = 25;
static const int WOUNDS = 5;
static const int POINTS_PER_UNIT = 90;

bool Runelord::s_registered = false;

Runelord::Runelord() :
    CitizenOfSigmar("Runelord", 4, WOUNDS, 7, 4, false),
    m_runeStaff(Weapon::Type::Melee, "Rune Staff", 1, 1, 4, 3, 0, RAND_D3),
    m_forgehammer(Weapon::Type::Melee, "Forgehammer", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HERO, PRIEST, RUNELORD};
    m_weapons = {&m_runeStaff, &m_forgehammer};

    // Runes of Spellbreaking
    m_totalUnbinds = 1;
}

bool Runelord::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_runeStaff);
    model->addMeleeWeapon(&m_forgehammer);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Runelord::Create(const ParameterList &parameters)
{
    auto unit = new Runelord();

    auto city = (City)GetEnumParam("City", parameters, CitizenOfSigmar::Hammerhal);
    unit->setCity(city);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Runelord::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Runelord::Create,
            CitizenOfSigmar::ValueToString,
            CitizenOfSigmar::EnumStringToInt,
            Runelord::ComputePoints,
            {
            },
            ORDER,
            { CITIES_OF_SIGMAR }
        };
        s_registered = UnitFactory::Register("Runelord", factoryMethod);
    }
}

int Runelord::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace CitiesOfSigmar