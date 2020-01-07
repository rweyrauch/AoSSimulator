/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/ArkanautAdmiral.h>
#include <UnitFactory.h>

namespace KharadronOverlords
{
static FactoryMethod factoryMethod = {
    ArkanautAdmiral::Create,
    KharadronBase::ValueToString,
    KharadronBase::EnumStringToInt,
    {
        {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
    },
    ORDER,
    KHARADRON_OVERLORDS
};

bool ArkanautAdmiral::s_registered = false;

Unit *ArkanautAdmiral::Create(const ParameterList &parameters)
{
    auto unit = new ArkanautAdmiral();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ArkanautAdmiral::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Arkanaut Admiral", factoryMethod);
    }
}

ArkanautAdmiral::ArkanautAdmiral() :
    KharadronBase("Arkanaut Admiral", 4, WOUNDS, 8, 3, false),
    m_pistol(Weapon::Type::Missile, "Volley Pistol", 9, 3, 3, 4, -1, 1),
    m_skalfhammer(Weapon::Type::Melee, "Skalfhammer", 1, 3, 3, 2, -2, 2)
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, ARKANAUT_ADMIRAL};
    m_weapons = {&m_pistol, &m_skalfhammer};
}

bool ArkanautAdmiral::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_pistol);
    model->addMeleeWeapon(&m_skalfhammer);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //KharadronOverlords
