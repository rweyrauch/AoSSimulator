/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "mawtribes/Butcher.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    Butcher::Create,
    Butcher::ValueToString,
    Butcher::EnumStringToInt,
    {
        {ParamType::Enum, "Weapon", Butcher::Cleaver, Butcher::Tenderiser, Butcher::Cleaver, 1},
    },
    DESTRUCTION,
    { OGOR_MAWTRIBES }
};

bool Butcher::s_registered = false;

Unit *Butcher::Create(const ParameterList &parameters)
{
    auto unit = new Butcher();

    auto weapon = (WeaponOption)GetEnumParam("Weapon", parameters, Cleaver);

    bool ok = unit->configure(weapon);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}


void Butcher::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Butcher", factoryMethod);
    }
}

Butcher::Butcher() :
    MawtribesBase("Butcher", 6, WOUNDS, 8, 5, false),
    m_tenderizer(Weapon::Type::Melee, "Tenderizer", 1, 3, 3, 3, -1, 3),
    m_cleaver(Weapon::Type::Melee, "Cleaver", 1, 3, 3, 3, -2, 2),
    m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, HERO, WIZARD, BUTCHER};
    m_weapons = {&m_tenderizer, &m_cleaver, &m_bite};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool Butcher::configure(WeaponOption weaponOption)
{
    auto model = new Model(BASESIZE, wounds());

    if (weaponOption == Tenderiser)
    {
        model->addMeleeWeapon(&m_tenderizer);
    }
    else if (weaponOption == Cleaver)
    {
        model->addMeleeWeapon(&m_cleaver);
    }
    model->addMeleeWeapon(&m_bite);

    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    //m_knownSpells.push_back(std::make_unique<VoraciousMaw>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

std::string Butcher::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == Tenderiser) return "Tenderiser";
        else if (parameter.intValue == Cleaver) return "Cleaver";
    }
    return ParameterValueToString(parameter);
}

int Butcher::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Tenderiser") return Tenderiser;
    else if (enumString == "Cleaver") return Cleaver;

    return 0;
}

} // namespace OgorMawtribes
