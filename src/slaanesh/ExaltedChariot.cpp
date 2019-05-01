/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/ExaltedChariot.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    ExaltedChariot::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    SLAANESH
};

bool ExaltedChariot::s_registered = false;

ExaltedChariot::ExaltedChariot() :
    Unit("Exalted Chariot", 10, WOUNDS, 10, 4, false),
    m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 4, 3, 4, -1, 1),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 9, 3, 4, -1, 1),
    m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 8, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, EXALTED_CHARIOT};
}

bool ExaltedChariot::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_flensingWhips);
    model.addMeleeWeapon(&m_piercingClaws);
    model.addMeleeWeapon(&m_poisonedTongues);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void ExaltedChariot::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_flensingWhips);
    visitor(&m_piercingClaws);
    visitor(&m_poisonedTongues);
}

Unit *ExaltedChariot::Create(const ParameterList &parameters)
{
    auto unit = new ExaltedChariot();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ExaltedChariot::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Exalted Chariot", factoryMethod);
    }
}

void ExaltedChariot::onCharged()
{
    Unit::onCharged();

    // Excess of Blades
    Dice dice;
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1.0f);
    for (auto ip : units)
    {
        int roll = dice.rollD6();
        if (roll >= 5)
        {
            ip->applyDamage({0, dice.rollD6()});
        }
        else if (dice.rollD6() >= 2)
        {
            ip->applyDamage({0, dice.rollD3()});
        }
    }
}

} // Slannesh