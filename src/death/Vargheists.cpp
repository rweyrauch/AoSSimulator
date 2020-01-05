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
        {
            ParamType::Integer, "Models", Vargheists::MIN_UNIT_SIZE, Vargheists::MIN_UNIT_SIZE, Vargheists::MAX_UNIT_SIZE,
            Vargheists::MIN_UNIT_SIZE
        },
    },
    DEATH,
    SOULBLIGHT
};

bool Vargheists::s_registered = false;

Vargheists::Vargheists() :
    Unit("Vargheists", 12, WOUNDS, 10, 5, true),
    m_fangsAndTalons(Weapon::Type::Melee, "Murderous Fangs and Talons", 1, 3, 3, 3, -1, 2),
    m_fangsAndTalonsVargoyle(Weapon::Type::Melee, "Murderous Fangs and Talons", 1, 4, 3, 3, -1, 2)
{
    m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, VARGHEISTS};
    m_weapons = {&m_fangsAndTalons, &m_fangsAndTalonsVargoyle};
}

bool Vargheists::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    auto vargoyle = new Model(BASESIZE, WOUNDS);
    vargoyle->addMeleeWeapon(&m_fangsAndTalonsVargoyle);
    addModel(vargoyle);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_fangsAndTalons);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *Vargheists::Create(const ParameterList &parameters)
{
    auto unit = new Vargheists();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
