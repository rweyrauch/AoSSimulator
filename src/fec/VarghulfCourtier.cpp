/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/VarghulfCourtier.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    VarghulfCourtier::Create,
    FleshEaterCourts::ValueToString,
    FleshEaterCourts::EnumStringToInt,
    {
        {ParamType::Enum, "Grand Court", FleshEaterCourts::NoCourt, FleshEaterCourts::NoCourt, FleshEaterCourts::Gristlegore, 1},
        {ParamType::Enum, "Delusion", FleshEaterCourts::None, FleshEaterCourts::None, FleshEaterCourts::DefendersOfTheRealm, 1},
    },
    DEATH,
    FLESH_EATER_COURTS
};

bool VarghulfCourtier::s_registered = false;

VarghulfCourtier::VarghulfCourtier() :
    FleshEaterCourts("Varghulf Courtier", 12, WOUNDS, 10, 4, true),
    m_immenseClaws(Weapon::Type::Melee, "Immense Claws", 2, 4, 3, 3, -1, 2),
    m_daggerlikeFangs(Weapon::Type::Melee, "Dagger-like Fangs", 1, 1, 3, 2, -2, RAND_D3)
{
    m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, COURTIER, HERO, VARGHULF_COURTIER};
}

bool VarghulfCourtier::configure()
{
    Model courtier(BASESIZE, WOUNDS);
    courtier.addMeleeWeapon(&m_immenseClaws);
    courtier.addMeleeWeapon(&m_daggerlikeFangs);
    addModel(courtier);

    m_points = POINTS_PER_UNIT;

    return true;
}

void VarghulfCourtier::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_immenseClaws);
    visitor(&m_daggerlikeFangs);
}

Unit *VarghulfCourtier::Create(const ParameterList &parameters)
{
    auto unit = new VarghulfCourtier();

    auto court = (GrandCourt)GetEnumParam("Grand Court", parameters, NoCourt);
    auto delusion = (Delusion)GetEnumParam("Delusion", parameters, None);
    // TODO: error checks - can only select delusion if GrandCourt is NoCourt.
    unit->setGrandCourt(court);
    unit->setCourtsOfDelusion(delusion);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VarghulfCourtier::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Varghulf Courtier", factoryMethod);
    }
}

} // namespace FleshEaterCourt
