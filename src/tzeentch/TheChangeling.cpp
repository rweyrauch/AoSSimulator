/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/TheChangeling.h"

namespace Tzeentch
{

bool TheChangeling::s_registered = false;

Unit *TheChangeling::Create(const ParameterList &parameters)
{
    auto unit = new TheChangeling();

    auto coven = (ChangeCoven)GetEnumParam("Change Coven", parameters, TzeentchBase::None);
    unit->setChangeCoven(coven);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void TheChangeling::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            TheChangeling::Create,
            TzeentchBase::ValueToString,
            TzeentchBase::EnumStringToInt,
            TheChangeling::ComputePoints,
            {
                {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None, TzeentchBase::GuildOfSummoners, 1},
            },
            CHAOS,
            { TZEENTCH }
        };
        s_registered = UnitFactory::Register("The Changeling", factoryMethod);
    }
}

TheChangeling::TheChangeling() :
    TzeentchBase("The Changeling", 6, WOUNDS, 7, 5, false),
    m_staff(Weapon::Type::Melee, "The Trickster's Staff", 2, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, THE_CHANGELING};
    m_weapons = {&m_staff};

    m_totalSpells = 2;
    m_totalUnbinds = 2;
}

bool TheChangeling::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_staff);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // Tzeentch