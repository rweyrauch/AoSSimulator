/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Battlesmith.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{

bool Battlesmith::s_registered = false;

Battlesmith::Battlesmith() :
    Fyreslayer("Battlesmith", 4, WOUNDS, 7, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_battleAxe(Weapon::Type::Melee, "Ancestral Battle-axe", 1, 3, 3, 3, -1, 2)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, TOTEM, BATTLESMITH};
    m_weapons = {&m_throwingAxe, &m_battleAxe};
}

bool Battlesmith::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMissileWeapon(&m_throwingAxe);
    model->addMeleeWeapon(&m_battleAxe);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Battlesmith::Create(const ParameterList &parameters)
{
    auto unit = new Battlesmith();

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

void Battlesmith::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Battlesmith::Create,
            Fyreslayer::ValueToString,
            Fyreslayer::EnumStringToInt,
            Battlesmith::ComputePoints,
            {
                {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
            },
            ORDER,
            { FYRESLAYERS }
        };
        s_registered = UnitFactory::Register("Battlesmith", factoryMethod);
    }
}

} // namespace Fyreslayers