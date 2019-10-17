/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Doomseeker.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    Doomseeker::Create,
    Fyreslayer::ValueToString,
    Fyreslayer::EnumStringToInt,
    {
        {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
    },
    ORDER,
    FYRESLAYERS
};

bool Doomseeker::s_registered = false;

Doomseeker::Doomseeker() :
    Fyreslayer("Doomseeker", 4, WOUNDS, 8, 4, false),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
    m_warIron(Weapon::Type::Melee, "Runic War-iron", 1, 3, 3, 4, 0, 1),
    m_doomseekerAxe(Weapon::Type::Melee, "Doomseeker Axe", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, DOOMSEEKER};
}

bool Doomseeker::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_throwingAxe);
    model.addMeleeWeapon(&m_warIron);
    model.addMeleeWeapon(&m_doomseekerAxe);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Doomseeker::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_throwingAxe);
    visitor(m_warIron);
    visitor(m_doomseekerAxe);
}

Unit *Doomseeker::Create(const ParameterList &parameters)
{
    auto unit = new Doomseeker();

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

void Doomseeker::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Doomseeker", factoryMethod);
    }
}

Wounds Doomseeker::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Runic Power
    if (!weapon->isMissile())
    {
        if (wounds() - remainingWounds() == 1)
        {
            return {weapon->damage()+1, 0};
        }
        else if (wounds() - remainingWounds() >= 2)
        {
            return {weapon->damage()+2, 0};
        }
    }
    return Fyreslayer::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace Fyreslayers