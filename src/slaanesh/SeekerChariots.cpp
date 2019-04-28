/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/SeekerChariots.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Slaanesh
{
static FactoryMethod factoryMethod = {
    SeekerChariots::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", SeekerChariots::MIN_UNIT_SIZE, SeekerChariots::MIN_UNIT_SIZE,
            SeekerChariots::MAX_UNIT_SIZE, SeekerChariots::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    SLAANESH
};

bool SeekerChariots::s_registered = false;

SeekerChariots::SeekerChariots() :
    Unit("Seeker Chariots", 12, WOUNDS, 10, 4, false),
    m_flensingWhips(Weapon::Type::Melee, "Flensing Whips", 2, 4, 3, 4, -1, 1),
    m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 3, 3, 4, -1, 1),
    m_poisonedTongues(Weapon::Type::Melee, "Poisoned Tongues", 1, 4, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, SEEKER_CHARIOTS};

    // Impossibly Swift
    m_retreatAndCharge = true;
}

bool SeekerChariots::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_flensingWhips);
        model.addMeleeWeapon(&m_piercingClaws);
        model.addMeleeWeapon(&m_poisonedTongues);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void SeekerChariots::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_flensingWhips);
    visitor(&m_piercingClaws);
    visitor(&m_poisonedTongues);
}

Unit *SeekerChariots::Create(const ParameterList &parameters)
{
    auto unit = new SeekerChariots();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SeekerChariots::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Seeker Chariots", factoryMethod);
    }
}

void SeekerChariots::onCharged()
{
    Unit::onCharged();

    // Multilating Blades
    Dice dice;
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1.0f);
    for (auto ip : units)
    {
        if (dice.rollD6() >= 2)
        {
            ip->applyDamage({0, dice.rollD3()});
        }
    }
}

} // Slannesh
