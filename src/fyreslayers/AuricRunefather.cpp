/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunefather.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    AuricRunefather::Create,
    Fyreslayer::ValueToString,
    Fyreslayer::EnumStringToInt,
    {
        {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
    },
    ORDER,
    FYRESLAYERS
};

bool AuricRunefather::s_registered = false;

AuricRunefather::AuricRunefather() :
    Fyreslayer("Auric Runefather", 4, WOUNDS, 8, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_grandAxe(Weapon::Type::Melee, "Latchkey Grandaxe", 3, 3, 3, 3, -1, 3)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, AURIC_RUNEFATHER};
}

bool AuricRunefather::configure()
{
        auto model = new Model(BASESIZE, WOUNDS);

    model->addMissileWeapon(&m_throwingAxe);
    model->addMeleeWeapon(&m_grandAxe);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void AuricRunefather::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_throwingAxe);
    visitor(m_grandAxe);
}

Unit *AuricRunefather::Create(const ParameterList &parameters)
{
    auto unit = new AuricRunefather();

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

void AuricRunefather::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Auric Runefather", factoryMethod);
    }
}

} // namespace Fyreslayers