/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageOrruks.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageOrruks::Create,
    SavageOrruks::ValueToString,
    SavageOrruks::EnumStringToInt,
    {
        {ParamType::Integer, "Models", SavageOrruks::MIN_UNIT_SIZE, SavageOrruks::MIN_UNIT_SIZE, SavageOrruks::MAX_UNIT_SIZE, SavageOrruks::MIN_UNIT_SIZE},
        {ParamType::Enum, "Weapons", SavageOrruks::Chompa, SavageOrruks::Chompa, SavageOrruks::SavageStikka, 1},
        {ParamType::Boolean, "Skull Thumper", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Bone Totem Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Warclan", Bonesplitterz::Bonegrinz, Bonesplitterz::Bonegrinz, Bonesplitterz::Icebone, 1},
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageOrruks::s_registered = false;

Unit *SavageOrruks::Create(const ParameterList &parameters)
{
    auto unit = new SavageOrruks();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, Chompa);
    bool thumper = GetBoolParam("Skull Thumper", parameters, true);
    bool totem = GetBoolParam("Bone Totem Bearer", parameters, true);

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Bonesplitterz::Bonegrinz);
    unit->setWarclan(warclan);

    bool ok = unit->configure(numModels, weapons, thumper, totem);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SavageOrruks::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Orruks", factoryMethod);
    }
}

SavageOrruks::SavageOrruks() :
    Bonesplitterz("Savage Orruks", 5, WOUNDS, 5, 6, false),
    m_chompa(Weapon::Type::Melee, "Chompa", 1, 2, 4, 3, 0, 1),
    m_stikka(Weapon::Type::Melee, "Savage Stikka", 2, 2, 4, 4, 0, 1),
    m_chompaBoss(Weapon::Type::Melee, "Chompa", 1, 3, 4, 3, 0, 1),
    m_stikkaBoss(Weapon::Type::Melee, "Savage Stikka", 2, 3, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, SAVAGE_ORRUKS};
}

bool SavageOrruks::configure(int numModels, WeaponOption weapons, bool skullThumper, bool totemBearer)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_thumper = skullThumper;
    m_totemBearer = totemBearer;

    // Add the Boss
    auto bossModel = new Model(BASESIZE, WOUNDS);
    if (weapons == Chompa)
    {
        bossModel->addMeleeWeapon(&m_chompaBoss);
    }
    else if (weapons == SavageStikka)
    {
        bossModel->addMeleeWeapon(&m_stikkaBoss);
    }
    addModel(bossModel);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == Chompa)
        {
            model->addMeleeWeapon(&m_chompa);
        }
        else if (weapons == SavageStikka)
        {
            model->addMeleeWeapon(&m_stikka);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SavageOrruks::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_chompa);
    visitor(m_chompaBoss);
    visitor(m_stikka);
    visitor(m_stikkaBoss);
}

std::string SavageOrruks::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == Chompa) { return "Chompa"; }
        else if (parameter.m_intValue == SavageStikka) { return "Savage Stikka"; }
    }
    return Bonesplitterz::ValueToString(parameter);
}

int SavageOrruks::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Chompa") { return Chompa; }
    else if (enumString == "Savage Stikka") { return SavageStikka; }
    return Bonesplitterz::EnumStringToInt(enumString);
}

int SavageOrruks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    // Spirit of Gorkamorka
    if (remainingModels() >= 15)
    {
        return 1;
    }
    return Unit::extraAttacks(attackingModel, weapon, target);
}

} // namespace Bonesplitterz
