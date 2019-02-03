/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/BloodSisters.h>
#include <UnitFactory.h>
#include <iostream>

namespace DaughtersOfKhaine {

static FactoryMethod factoryMethod = {
    BloodSisters::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = BloodSisters::MIN_UNIT_SIZE}, BloodSisters::MIN_UNIT_SIZE,
         BloodSisters::MAX_UNIT_SIZE, BloodSisters::MIN_UNIT_SIZE},
    }
};

Weapon BloodSisters::s_heartshardGlaive(Weapon::Type::Melee, "Heartshard Glaive", 2, 3, 3, 3, -1, 1);
Weapon BloodSisters::s_heartshardGlaiveGorgai(Weapon::Type::Melee, "Heartshard Glaive (Gorgai)", 2, 3, 2, 3, -1, 1);
Weapon BloodSisters::s_crystalTouch(Weapon::Type::Melee, "Crystal Touch", 1, 1, 4, 0, 0, 1);
Weapon BloodSisters::s_crystalTouchGorgai(Weapon::Type::Melee, "Crystal Touch", 1, 1, 3, 0, 0, 1);

bool BloodSisters::s_registered = false;

BloodSisters::BloodSisters() :
    DaughterOfKhaine("Blood Sisters", 8, WOUNDS, 8, 5, false)
{
    m_keywords = { ORDER, AELF, DAUGHTERS_OF_KHAINE, MELUSAI, BLOOD_SISTERS };
}

bool BloodSisters::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model gorgai(BASESIZE, WOUNDS);
    gorgai.addMeleeWeapon(&s_heartshardGlaiveGorgai);
    gorgai.addMeleeWeapon(&s_crystalTouchGorgai);
    addModel(gorgai);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_heartshardGlaive);
        model.addMeleeWeapon(&s_crystalTouch);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

Unit *BloodSisters::Create(const ParameterList &parameters)
{
    auto unit = new BloodSisters();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BloodSisters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Blood Sisters", factoryMethod);
    }
}

int BloodSisters::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits, const WoundingHits& wounds)
{
    // Turned to Crystal
    if (weapon->name() == s_crystalTouch.name())
    {
        return hits.numHits;
    }

    return Unit::generateMortalWounds(weapon, unit, hits, wounds);
}

void BloodSisters::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_heartshardGlaive);
    visitor(&s_heartshardGlaiveGorgai);
}

} // namespace DaughtersOfKhaine