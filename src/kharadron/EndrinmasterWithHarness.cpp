/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/EndrinmasterWithHarness.h>
#include <UnitFactory.h>

namespace KharadronOverlords
{
static FactoryMethod factoryMethod = {
    EndrinmasterWithEndrinharness::Create,
    KharadronBase::ValueToString,
    KharadronBase::EnumStringToInt,
    {
        {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
    },
    ORDER,
    KHARADRON_OVERLORDS
};

bool EndrinmasterWithEndrinharness::s_registered = false;

Unit *EndrinmasterWithEndrinharness::Create(const ParameterList &parameters)
{
    auto unit = new EndrinmasterWithEndrinharness();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void EndrinmasterWithEndrinharness::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Endrinmaster with Endrinharness", factoryMethod);
    }
}

EndrinmasterWithEndrinharness::EndrinmasterWithEndrinharness() :
    KharadronBase("Endrinmaster with Endrinharness", 4, WOUNDS, 7, 4, false),
    m_gaze(Weapon::Type::Missile, "Gaze of Grungni", 9, 1, 3, 2, -1, RAND_D3),
    m_hammer(Weapon::Type::Melee, "Aethermight Hammer", 1, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, ENDRINMASTER};
    m_weapons = {&m_gaze, &m_hammer};
}

bool EndrinmasterWithEndrinharness::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_gaze);
    model->addMeleeWeapon(&m_hammer);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //KharadronOverlords