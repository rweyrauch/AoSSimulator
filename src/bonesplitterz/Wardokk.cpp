/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "bonesplitterz/Wardokk.h"

namespace Bonesplitterz
{

bool Wardokk::s_registered = false;

Unit *Wardokk::Create(const ParameterList &parameters)
{
    auto unit = new Wardokk();

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Bonesplitterz::Bonegrinz);
    unit->setWarclan(warclan);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Wardokk::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            Bonesplitterz::ValueToString,
            Bonesplitterz::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Warclan", Bonesplitterz::Bonegrinz, Bonesplitterz::Bonegrinz, Bonesplitterz::Icebone, 1},
            },
            DESTRUCTION,
            { BONESPLITTERZ }
        };

        s_registered = UnitFactory::Register("Wardokk", factoryMethod);
    }
}

Wardokk::Wardokk() :
    Bonesplitterz("Wardokk", 5, WOUNDS, 7, 6, false),
    m_bonebeastStikk(Weapon::Type::Melee, "Bonebeast Stikk", 1, 1, 4, 3, 0, RAND_D3)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, PRIEST, WIZARD, WARDOKK};
    m_weapons = {&m_bonebeastStikk};
    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool Wardokk::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_bonebeastStikk);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} // namespace Bonesplitterz
