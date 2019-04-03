/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/BloodStalkers.h>
#include <UnitFactory.h>
#include <iostream>

namespace DaughtersOfKhaine
{

static FactoryMethod factoryMethod = {
    BloodStalkers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", BloodStalkers::MIN_UNIT_SIZE, BloodStalkers::MIN_UNIT_SIZE,
            BloodStalkers::MAX_UNIT_SIZE, BloodStalkers::MIN_UNIT_SIZE
        },
        {ParamType ::Boolean, "Blood Wyrm", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
    },
    ORDER,
    DAUGHTERS_OF_KHAINE
};


bool BloodStalkers::s_registered = false;

BloodStalkers::BloodStalkers() :
    DaughterOfKhaine("Blood Stalkers", 8, WOUNDS, 8, 5, false),
    m_hearseekerBow(Weapon::Type::Missile, "Heartseeker Bow", 24, 1, 3, 3, -1, 1),
    m_sacraficialDaggar(Weapon::Type::Melee, "Sacrificial Daggar", 1, 2, 3, 4, 0, 1),
    m_heartseekBowKrone(Weapon::Type::Missile, "Heartseeker Bow (Krone)", 24, 1, 2, 3, -1, 1),
    m_sacraficialDaggarKrone(Weapon::Type::Melee, "Sacrificial Daggar (Krone)", 1, 2, 2, 4, 0, 1),
    m_bloodWyrm(Weapon::Type::Melee, "Blood Wyrm", 1, 1, 3, 3, 0, 1)
{
    m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, MELUSAI, BLOOD_STALKERS};
}

bool BloodStalkers::configure(int numModels, bool bloodWyrm)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model krone(BASESIZE, WOUNDS);
    krone.addMissileWeapon(&m_heartseekBowKrone);
    krone.addMeleeWeapon(&m_sacraficialDaggarKrone);
    if (bloodWyrm)
    {
        krone.addMeleeWeapon(&m_bloodWyrm);
    }
    addModel(krone);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_hearseekerBow);
        model.addMeleeWeapon(&m_sacraficialDaggar);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *BloodStalkers::Create(const ParameterList &parameters)
{
    auto unit = new BloodStalkers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool bloodWyrm = GetBoolParam("Blood Wyrm", parameters, false);

    bool ok = unit->configure(numModels, bloodWyrm);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BloodStalkers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Blood Stalkers", factoryMethod);
    }
}

Wounds BloodStalkers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Heartseekers
    if ((hitRoll >= 6) && (weapon->name() == m_hearseekerBow.name()))
    {
        return {0, 1};
    }

    return DaughterOfKhaine::weaponDamage(weapon, target, hitRoll, woundRoll);
}

void BloodStalkers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_hearseekerBow);
    visitor(&m_sacraficialDaggar);
    visitor(&m_heartseekBowKrone);
    visitor(&m_sacraficialDaggarKrone);
    visitor(&m_bloodWyrm);
}

} // namespace DaughtersOfKhaine