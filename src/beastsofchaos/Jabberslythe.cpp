/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Jabberslythe.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{

static FactoryMethod factoryMethod = {
    Jabberslythe::Create,
    BeastsOfChaosBase::ValueToString,
    BeastsOfChaosBase::EnumStringToInt,
    {
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool Jabberslythe::s_registered = false;

Jabberslythe::Jabberslythe() :
    BeastsOfChaosBase("Jabberslythe", 12, WOUNDS, 6, 5, true),
    m_slytheyTongue(Weapon::Type::Missile, "Slythey Tongue", 9, 1, 3, 3, -1, RAND_D3),
    m_vorpalClaws(Weapon::Type::Melee, "Vorpal Claws", 1, 6, 3, 3, -2, 1),
    m_spikedTail(Weapon::Type::Melee, "Spiked Tail", 3, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, JABBERSLYTHE};
}

bool Jabberslythe::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_slytheyTongue);
    model.addMeleeWeapon(&m_vorpalClaws);
    model.addMeleeWeapon(&m_spikedTail);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Jabberslythe::Create(const ParameterList &parameters)
{
    auto unit = new Jabberslythe();

    auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
    unit->setGreatfray(fray);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Jabberslythe::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Jabberslythe", factoryMethod);
    }
}

void Jabberslythe::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_slytheyTongue);
    visitor(&m_vorpalClaws);
    visitor(&m_spikedTail);
}

} // namespace BeastsOfChaos