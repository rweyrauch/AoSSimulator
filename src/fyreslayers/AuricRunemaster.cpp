/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunemaster.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    AuricRunemaster::Create,
    Fyreslayer::ValueToString,
    Fyreslayer::EnumStringToInt,
    {
        {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
    },
    ORDER,
    FYRESLAYERS
};

bool AuricRunemaster::s_registered = false;

AuricRunemaster::AuricRunemaster() :
    Fyreslayer("Auric Runemaster", 4, WOUNDS, 8, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_brazierStaff(Weapon::Type::Melee, "Brazier-staff", 2, 1, 4, 3, -1, RAND_D3),
    m_runicIron(Weapon::Type::Melee, "Runic Iron", 1, 2, 3, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, PRIEST, AURIC_RUNEMASTER};
    m_weapons = {&m_throwingAxe, &m_brazierStaff, &m_runicIron};
}

bool AuricRunemaster::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_throwingAxe);
    model->addMeleeWeapon(&m_brazierStaff);
    model->addMeleeWeapon(&m_runicIron);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *AuricRunemaster::Create(const ParameterList &parameters)
{
    auto unit = new AuricRunemaster();

    auto lodge = (Lodge) GetEnumParam("Lodge", parameters, Fyreslayer::None);
    unit->setLodge(lodge);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AuricRunemaster::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Auric Runemaster", factoryMethod);
    }
}

} // namespace Fyreslayers