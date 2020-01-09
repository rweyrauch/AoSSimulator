/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/EndrinmasterWithSuit.h>
#include <UnitFactory.h>

namespace KharadronOverlords
{
static FactoryMethod factoryMethod = {
    EndrinmasterWithDirigibleSuit::Create,
    KharadronBase::ValueToString,
    KharadronBase::EnumStringToInt,
    {
        {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
    },
    ORDER,
    KHARADRON_OVERLORDS
};

bool EndrinmasterWithDirigibleSuit::s_registered = false;

Unit *EndrinmasterWithDirigibleSuit::Create(const ParameterList &parameters)
{
    auto unit = new EndrinmasterWithDirigibleSuit();

    auto port = (Skyport)GetEnumParam("Skyport", parameters, KharadronBase::None);
    unit->setSkyport(port);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void EndrinmasterWithDirigibleSuit::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Endrinmaster with Dirigible Suit", factoryMethod);
    }
}

EndrinmasterWithDirigibleSuit::EndrinmasterWithDirigibleSuit() :
    KharadronBase("Endrinmaster with Dirigible Suit", 12, WOUNDS, 8, 3, true),
    m_aethercannon(Weapon::Type::Missile, "Aethercannon", 12, 1, 3, 2, -2, RAND_D3),
    m_weaponBattery(Weapon::Type::Missile, "Dirigible Suit Weapon Battery", 18, 6, 3, 3, -1, 1),
    m_gaze(Weapon::Type::Missile, "Gaze of Grungni", 9, 1, 3, 2, -1, RAND_D3),
    m_saw(Weapon::Type::Melee, "Aethermatic Saw", 1, 3, 3, 2, -2, RAND_D3)
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, ENDRINMASTER};
    m_weapons = {&m_aethercannon, &m_weaponBattery, &m_gaze, &m_saw};
}

bool EndrinmasterWithDirigibleSuit::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_aethercannon);
    model->addMissileWeapon(&m_weaponBattery);
    model->addMissileWeapon(&m_gaze);
    model->addMeleeWeapon(&m_saw);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //KharadronOverlords
