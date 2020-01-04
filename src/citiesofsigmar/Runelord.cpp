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
static FactoryMethod factoryMethod = {
    Runelord::Create,
    CitizenOfSigmar::ValueToString,
    CitizenOfSigmar::EnumStringToInt,
    {
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool Runelord::s_registered = false;

Runelord::Runelord() :
    CitizenOfSigmar("Runelord", 4, WOUNDS, 7, 4, false),
    m_runeStaff(Weapon::Type::Melee, "Runic Axes", 1, 6, 3, 3, -1, 1),
    m_forgehammer()
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HERO, PRIEST, RUNELORD};

    // Runes of Spellbreaking
    m_totalUnbinds = 1;
}

bool Runelord::configure()
{
        auto model = new Model(BASESIZE, WOUNDS);

    model->addMeleeWeapon(&m_runeStaff);
    model->addMeleeWeapon(&m_forgehammer);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Runelord::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_runeStaff);
    visitor(m_forgehammer);
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
        s_registered = UnitFactory::Register("Runelord", factoryMethod);
    }
}

} // namespace CitiesOfSigmar