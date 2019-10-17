/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/DragonOgorShaggoth.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{

static FactoryMethod factoryMethod = {
    DragonOgorShaggoth::Create,
    BeastsOfChaosBase::ValueToString,
    BeastsOfChaosBase::EnumStringToInt,
    {
        {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None, BeastsOfChaosBase::Gavespawn, 1},
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool DragonOgorShaggoth::s_registered = false;

DragonOgorShaggoth::DragonOgorShaggoth() :
    BeastsOfChaosBase("Dragon Ogor Shaggoth", 8, WOUNDS, 7, 4, false),
    m_stormWroughtAxe(Weapon::Type::Melee, "Storm-wrought Axe", 2, 3, 3, 3, -1, 3),
    m_sweepingTail(Weapon::Type::Melee, "Sweeping Tail", 3, RAND_D3, 4, 3, 0, 1),
    m_talonedForelimbs(Weapon::Type::Melee, "Taloned Forelimbs", 1, 2, 3, 3, -1, 1)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, THUNDERSCORN, DRAGON_OGOR, MONSTER, HERO, WIZARD, SHAGGOTH};
}

bool DragonOgorShaggoth::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_stormWroughtAxe);
    model.addMeleeWeapon(&m_sweepingTail);
    model.addMeleeWeapon(&m_talonedForelimbs);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *DragonOgorShaggoth::Create(const ParameterList &parameters)
{
    auto unit = new DragonOgorShaggoth();

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

void DragonOgorShaggoth::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dragon Ogor Shaggoth", factoryMethod);
    }
}

void DragonOgorShaggoth::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_stormWroughtAxe);
    visitor(m_sweepingTail);
    visitor(m_talonedForelimbs);
}

} // namespace BeastsOfChaos
