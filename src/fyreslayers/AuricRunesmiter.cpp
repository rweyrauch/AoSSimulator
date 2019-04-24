/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunesmiter.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    AuricRunesmiter::Create,
    Fyreslayer::ValueToString,
    Fyreslayer::EnumStringToInt,
    {
        {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
    },
    ORDER,
    FYRESLAYERS
};

bool AuricRunesmiter::s_registered = false;

AuricRunesmiter::AuricRunesmiter() :
    Fyreslayer("Auric Runesmiter", 4, WOUNDS, 7, 5, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_runicIron(Weapon::Type::Melee, "Runic Iron", 1, 2, 3, 4, 0, 1),
    m_latchAxe(Weapon::Type::Melee, "Latch-axe", 1, 1, 4, 3, 0, 2)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, PRIEST, AURIC_RUNESMITER};
}

bool AuricRunesmiter::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_throwingAxe);
    model.addMeleeWeapon(&m_latchAxe);
    model.addMeleeWeapon(&m_runicIron);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void AuricRunesmiter::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_throwingAxe);
    visitor(&m_latchAxe);
    visitor(&m_runicIron);
}

Unit *AuricRunesmiter::Create(const ParameterList &parameters)
{
    auto unit = new AuricRunesmiter();

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

void AuricRunesmiter::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Auric Runesmiter", factoryMethod);
    }
}

} // namespace Fyreslayers