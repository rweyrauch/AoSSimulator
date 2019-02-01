/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/Khorgoraths.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Khorgoraths::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Khorgoraths::MIN_UNIT_SIZE},
            Khorgoraths::MIN_UNIT_SIZE,
            Khorgoraths::MAX_UNIT_SIZE, Khorgoraths::MIN_UNIT_SIZE
        },
    }
};

Weapon Khorgoraths::s_boneTentacles(Weapon::Type::Missile, "Bone Tentacles", 6, 3, 3, 4, 0, 1);
Weapon Khorgoraths::s_clawAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 5, 3, 3, -1, 2);

bool Khorgoraths::s_registered = false;


Khorgoraths::Khorgoraths() :
    Unit("Khorgoraths", 6, WOUNDS, 6, 4, false)
{
    m_keywords = {CHAOS, KHORNE, MONSTER, BLOODBOUND, KHORGORATHS};
}

bool Khorgoraths::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&s_boneTentacles);
        model.addMeleeWeapon(&s_clawAndFangs);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Khorgoraths::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_boneTentacles);
    visitor(&s_clawAndFangs);
}

Unit *Khorgoraths::Create(const ParameterList &parameters)
{
    auto unit = new Khorgoraths();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Khorgoraths::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Khorgoraths", factoryMethod);
    }
}
} //namespace Khorne

