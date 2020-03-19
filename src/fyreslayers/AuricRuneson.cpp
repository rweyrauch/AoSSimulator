/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRuneson.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{

bool AuricRuneson::s_registered = false;

AuricRuneson::AuricRuneson() :
    Fyreslayer("Auric Runeson", 4, WOUNDS, 7, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_javelin(Weapon::Type::Missile, "Wyrmslayer Javelin", 12, 1, 4, 3, -1, RAND_D3),
    m_warAxe(Weapon::Type::Melee, "Ancestral War-axe", 1, 3, 3, 4, 0, RAND_D3),
    m_javelinMelee(Weapon::Type::Melee, "Wyrmslayer Javelin", 3, 1, 3, 3, -1, 1)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, AURIC_RUNESON};
    m_weapons = {&m_throwingAxe, &m_javelin, &m_warAxe, &m_javelinMelee};
}

bool AuricRuneson::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_throwingAxe);
    model->addMissileWeapon(&m_javelin);
    model->addMeleeWeapon(&m_warAxe);
    model->addMeleeWeapon(&m_javelinMelee);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *AuricRuneson::Create(const ParameterList &parameters)
{
    auto unit = new AuricRuneson();

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

void AuricRuneson::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            AuricRuneson::Create,
            Fyreslayer::ValueToString,
            Fyreslayer::EnumStringToInt,
            AuricRuneson::ComputePoints,
            {
                {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
            },
            ORDER,
            { FYRESLAYERS }
        };
        s_registered = UnitFactory::Register("Auric Runeson", factoryMethod);
    }
}

Rerolls AuricRuneson::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 6.0f);
    for (auto unit : units)
    {
        if (unit->hasKeyword(AURIC_RUNESON)) return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

Wounds AuricRuneson::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Wyrmslayer Javelins
    if ((weapon->name() == m_javelin.name()) && target->hasKeyword(MONSTER))
    {
        return {weapon->damage()+2, 0};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace Fyreslayers