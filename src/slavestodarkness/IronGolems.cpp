/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/IronGolems.h"

namespace SlavesToDarkness
{
static FactoryMethod factoryMethod = {
    IronGolems::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", IronGolems::MIN_UNIT_SIZE, IronGolems::MIN_UNIT_SIZE,
            IronGolems::MAX_UNIT_SIZE, IronGolems::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    SLAVES_TO_DARKNESS
};

bool IronGolems::s_registered = false;

Unit *IronGolems::Create(const ParameterList &parameters)
{
    auto unit = new IronGolems();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void IronGolems::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Iron Golems", factoryMethod);
    }
}

IronGolems::IronGolems() :
    Unit("Iron Golems", 5, WOUNDS, 6, 4, false),
    m_bolas(Weapon::Type::Missile, "Bolas", 8, 1, 4, 4, 0, 1),
    m_legionWeapons(Weapon::Type::Melee, "Legion Weapons", 1, 1, 4, 4, 0, 1),
    m_legionWeaponsDominar(Weapon::Type::Melee, "Legion Weapons (Dominar)", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, IRON_GOLEMS};
}

bool IronGolems::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model dominar(BASESIZE, WOUNDS);
    dominar.addMissileWeapon(&m_bolas);
    dominar.addMeleeWeapon(&m_legionWeaponsDominar);
    dominar.setName("Dominar");
    addModel(dominar);

    Model signifer(BASESIZE, WOUNDS);
    signifer.addMissileWeapon(&m_bolas);
    signifer.addMeleeWeapon(&m_legionWeapons);
    signifer.setName("Signifer");
    addModel(signifer);

    Model breacher(BASESIZE, 3);
    breacher.addMissileWeapon(&m_bolas);
    breacher.addMeleeWeapon(&m_legionWeapons);
    breacher.setName("Ogor Breacher");
    addModel(breacher);

    for (auto i = 3; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_bolas);
        model.addMeleeWeapon(&m_legionWeapons);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void IronGolems::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_bolas);
    visitor(m_legionWeapons);
    visitor(m_legionWeaponsDominar);
}

int IronGolems::braveryModifier() const
{
    int modifier = Unit::braveryModifier();
    if (m_hasSignifer)
    {
        modifier += 2;
    }
    return modifier;
}

void IronGolems::onWounded()
{
    Unit::onWounded();

    // Check for Signifer
    for (const auto& ip : m_models)
    {
        if (ip.slain() && (ip.getName() == "Signifer"))
        {
            m_hasSignifer = false;
            break;
        }
    }

}

} //SlavesToDarkness