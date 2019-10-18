/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/BattlemageOnGriffon.h"

namespace CitiesOfSigmar
{
static FactoryMethod factoryMethod = {
    BattlemageOnGriffon::Create,
    BattlemageOnGriffon::ValueToString,
    BattlemageOnGriffon::EnumStringToInt,
    {
        {ParamType::Enum, "City", CitizenOfSigmar::Hammerhal, CitizenOfSigmar::Hammerhal, CitizenOfSigmar::TempestsEye, 1},
    },
    ORDER,
    CITIES_OF_SIGMAR
};

bool BattlemageOnGriffon::s_registered = false;

Unit *BattlemageOnGriffon::Create(const ParameterList &parameters)
{
    auto unit = new BattlemageOnGriffon();

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

std::string BattlemageOnGriffon::ValueToString(const Parameter &parameter)
{
    return CitizenOfSigmar::ValueToString(parameter);
}

int BattlemageOnGriffon::EnumStringToInt(const std::string &enumString)
{
    return CitizenOfSigmar::EnumStringToInt(enumString);
}

void BattlemageOnGriffon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Battlemage on Griffon", factoryMethod);
    }
}

BattlemageOnGriffon::BattlemageOnGriffon() :
    CitizenOfSigmar("Battlemage on Griffon", 15, WOUNDS, 6, 5, true),
    m_beastStaff(Weapon::Type::Melee, "Beaststaff", 2, 1, 4, 3, -1, RAND_D3),
    m_twinBeaks(Weapon::Type::Melee, "Twin Beaks", 2, 4, 3, 3, -1, 3),
    m_razorClaws(Weapon::Type::Melee, "Razor Claws", 2, 6, 4, 3, -1, 2)
{
    m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, MONSTER, HERO, WIZARD, BATTLEMAGE};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool BattlemageOnGriffon::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_beastStaff);
    model.addMeleeWeapon(&m_twinBeaks);
    model.addMeleeWeapon(&m_razorClaws);

    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void BattlemageOnGriffon::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_beastStaff);
    visitor(m_twinBeaks);
    visitor(m_razorClaws);
}

} //namespace CitiesOfSigmar