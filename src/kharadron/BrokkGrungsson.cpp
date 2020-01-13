/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/BrokkGrungsson.h>
#include <UnitFactory.h>

namespace KharadronOverlords
{
static FactoryMethod factoryMethod = {
    BrokkGrungsson::Create,
    KharadronBase::ValueToString,
    KharadronBase::EnumStringToInt,
    {
        {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
    },
    ORDER,
    { KHARADRON_OVERLORDS }
};

bool BrokkGrungsson::s_registered = false;

Unit *BrokkGrungsson::Create(const ParameterList &parameters)
{
    auto unit = new BrokkGrungsson();

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

void BrokkGrungsson::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Brokk Grungsson", factoryMethod);
    }
}

BrokkGrungsson::BrokkGrungsson() :
    KharadronBase("Brokk Grungsson", 12, WOUNDS, 8, 3, true),
    m_boast(Weapon::Type::Missile, "Grungsson's Boast", 18, 2, 3, 2, -2, RAND_D3),
    m_charter(Weapon::Type::Missile, "The Magnate's Charter", 18, 6, 3, 3, -1, 1),
    m_aetherblasters(Weapon::Type::Missile, "Aetherblasters", 9, 2, 3, 4, 0, 1),
    m_saw(Weapon::Type::Melee, "Aethermatic Saw", 1, 4, 3, 2, -2, RAND_D3)
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, BARAK_NAR, HERO, SKYFARER, BROKK_GRUNGSSON};
    m_weapons = {&m_boast, &m_charter, &m_aetherblasters, &m_saw};
}

bool BrokkGrungsson::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_boast);
    model->addMissileWeapon(&m_charter);
    model->addMissileWeapon(&m_aetherblasters);
    model->addMeleeWeapon(&m_saw);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //KharadronOverlords
