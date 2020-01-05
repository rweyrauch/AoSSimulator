/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/GrimwrathBerzerker.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    GrimwrathBerzerker::Create,
    Fyreslayer::ValueToString,
    Fyreslayer::EnumStringToInt,
    {
        {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
    },
    ORDER,
    FYRESLAYERS
};

bool GrimwrathBerzerker::s_registered = false;

GrimwrathBerzerker::GrimwrathBerzerker() :
    Fyreslayer("Grimwrath Berzerker", 4, WOUNDS, 9, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_greatAxe(Weapon::Type::Melee, "Fyrestorm Greataxe", 1, 4, 3, 3, -2, 2)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, GRIMWRATH_BERZERKER};
    m_weapons = {&m_throwingAxe, &m_greatAxe};
}

bool GrimwrathBerzerker::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_throwingAxe);
    model->addMeleeWeapon(&m_greatAxe);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *GrimwrathBerzerker::Create(const ParameterList &parameters)
{
    auto unit = new GrimwrathBerzerker();

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

void GrimwrathBerzerker::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Grimwrath Berzerker", factoryMethod);
    }
}

} // namespace Fyreslayers