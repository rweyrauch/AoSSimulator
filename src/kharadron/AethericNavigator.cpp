/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/AethericNavigator.h>
#include <UnitFactory.h>

namespace KharadronOverlords
{

static FactoryMethod factoryMethod = {
    AethericNavigator::Create,
    KharadronBase::ValueToString,
    KharadronBase::EnumStringToInt,
    {
        {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
    },
    ORDER,
    KHARADRON_OVERLORDS
};

bool AethericNavigator::s_registered = false;

Unit *AethericNavigator::Create(const ParameterList &parameters)
{
    auto unit = new AethericNavigator();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AethericNavigator::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Aetheric Navigator", factoryMethod);
    }
}

AethericNavigator::AethericNavigator() :
    KharadronBase("Aetheric Navigator", 4, WOUNDS, 7, 3, false),
    m_pistol(Weapon::Type::Missile, "Ranging Pistol", 15, 2, 3, 3, -1, 1),
    m_zephyrscope(Weapon::Type::Melee, "Zephyrscope", 1, 2, 3, 4, 0, 1)
{
    m_keywords ={ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, AETHERIC_NAVIGATOR};
    m_weapons = {&m_pistol, &m_zephyrscope};

    // Aethersight
    m_totalUnbinds = 1;
}

bool AethericNavigator::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_pistol);
    model->addMeleeWeapon(&m_zephyrscope);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //KharadronOverlords