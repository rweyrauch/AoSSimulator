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
static FactoryMethod factoryMethod = {
    AuricRuneson::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    FYRESLAYERS
};

bool AuricRuneson::s_registered = false;

AuricRuneson::AuricRuneson() :
    Unit("Auric Runeson", 4, WOUNDS, 7, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_javelin(Weapon::Type::Missile, "Wyrmslayer Javelin", 12, 1, 4, 3, -1, RAND_D3),
    m_warAxe(Weapon::Type::Melee, "Ancestral War-axe", 1, 3, 3, 4, 0, RAND_D3),
    m_javelinMelee(Weapon::Type::Melee, "Wyrmslayer Javelin", 3, 1, 3, 3, -1, 1)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, AURIC_RUNESON};
}

bool AuricRuneson::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_throwingAxe);
    model.addMissileWeapon(&m_javelin);
    model.addMeleeWeapon(&m_warAxe);
    model.addMeleeWeapon(&m_javelinMelee);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void AuricRuneson::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_throwingAxe);
    visitor(&m_javelin);
    visitor(&m_warAxe);
    visitor(&m_javelinMelee);
}

Unit *AuricRuneson::Create(const ParameterList &parameters)
{
    auto unit = new AuricRuneson();

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
        s_registered = UnitFactory::Register("Auric Runeson", factoryMethod);
    }
}

} // namespace Fyreslayers