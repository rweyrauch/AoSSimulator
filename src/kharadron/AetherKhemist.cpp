/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/AetherKhemist.h>
#include <UnitFactory.h>

namespace KharadronOverlords
{

bool AetherKhemist::s_registered = false;

Unit *AetherKhemist::Create(const ParameterList &parameters)
{
    auto unit = new AetherKhemist();

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

void AetherKhemist::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            KharadronBase::ValueToString,
            KharadronBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
            },
            ORDER,
            { KHARADRON_OVERLORDS }
        };

        s_registered = UnitFactory::Register("Aether Khemist", *factoryMethod);
    }
}

AetherKhemist::AetherKhemist() :
    KharadronBase("Aether Khemist", 4, WOUNDS, 7, 4, false),
    m_anatomiser(Weapon::Type::Missile, "Atmospheric Anatomiser", 9, RAND_3D6, 4, 4, -2, 1),
    m_instruments(Weapon::Type::Melee, "Heavy Instruments", 1, 2, 4, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, AETHER_KHEMIST};
    m_weapons = {&m_anatomiser, &m_instruments};

    s_globalToHitMod.connect(this, &AetherKhemist::atmosphericIsolation, &m_connection);
}

AetherKhemist::~AetherKhemist()
{
    m_connection.disconnect();
}

bool AetherKhemist::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_anatomiser);
    model->addMeleeWeapon(&m_instruments);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

int AetherKhemist::atmosphericIsolation(const Weapon *weapon, const Unit *target)
{
    // Atmospheric Isolation
    if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0f))
    {
        return -1;
    }

    return 0;
}

} //KharadronOverlords
