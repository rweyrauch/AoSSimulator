/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Vargheists.h>
#include <UnitFactory.h>

namespace Death
{
static FactoryMethod factoryMethod = {
    Vargheists::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = Vargheists::MIN_UNIT_SIZE}, Vargheists::MIN_UNIT_SIZE, Vargheists::MAX_UNIT_SIZE, Vargheists::MIN_UNIT_SIZE},
    }
};

Weapon Vargheists::s_fangsAndTalons(Weapon::Type::Melee, "Murderous Fangs and Talons", 1, 3, 3, 3, -1, 2);
Weapon Vargheists::s_fangsAndTalonsVargoyle(Weapon::Type::Melee, "Murderous Fangs and Talons (Vargoyle)", 1, 4, 3, 3, -1, 2);

bool Vargheists::s_registered = false;

Vargheists::Vargheists() :
    Unit("Vargheists", 12, WOUNDS, 10, 5, true)
{
    m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, VARGHEISTS};
}

bool Vargheists::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    Model vargoyle(BASESIZE, WOUNDS);
    vargoyle.addMeleeWeapon(&s_fangsAndTalonsVargoyle);
    addModel(vargoyle);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_fangsAndTalons);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void Vargheists::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_fangsAndTalons);
    visitor(&s_fangsAndTalonsVargoyle);
}

Unit *Vargheists::Create(const ParameterList &parameters)
{
    auto unit = new Vargheists();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Vargheists::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Vargheists", factoryMethod);
    }
}

} //namespace Death
