/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/BladebringerExaltedChariot.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    BladebringerOnExaltedChariot::Create,
    SlaaneshBase::ValueToString,
    SlaaneshBase::EnumStringToInt,
    {
        {ParamType::Enum, "Host", SlaaneshBase::Godseekers, SlaaneshBase::Invaders, SlaaneshBase::Godseekers, 1},
    },
    CHAOS,
    { SLAANESH }
};

bool BladebringerOnExaltedChariot::s_registered = false;

BladebringerOnExaltedChariot::BladebringerOnExaltedChariot() :
    SlaaneshBase("Bladebringer, Herald on Exalted Chariot", 10, WOUNDS, 10, 4, false),
    m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 6, 3, 4, -1, 1),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 9, 3, 4, -1, 1),
    m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 8, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH, EXALTED_CHARIOT, BLADEBRINGER};
    m_weapons = {&m_flensingWhips, &m_piercingClaws, &m_poisonedTongues};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool BladebringerOnExaltedChariot::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_flensingWhips);
    model->addMeleeWeapon(&m_piercingClaws);
    model->addMeleeWeapon(&m_poisonedTongues);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *BladebringerOnExaltedChariot::Create(const ParameterList &parameters)
{
    auto unit = new BladebringerOnExaltedChariot();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BladebringerOnExaltedChariot::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bladebringer, Herald on Exalted Chariot", factoryMethod);
    }
}

void BladebringerOnExaltedChariot::onCharged()
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
