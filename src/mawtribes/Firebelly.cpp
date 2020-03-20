/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "mawtribes/Firebelly.h"

namespace OgorMawtribes
{
static const int BASESIZE = 50;
static const int WOUNDS = 7;
static const int POINTS_PER_UNIT = 120;

bool Firebelly::s_registered = false;

Unit *Firebelly::Create(const ParameterList &parameters)
{
    auto unit = new Firebelly();

    auto tribe = (Mawtribe)GetEnumParam("Mawtribe", parameters, None);
    unit->setMawtribe(tribe);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}


void Firebelly::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Firebelly::Create,
            MawtribesBase::ValueToString,
            MawtribesBase::EnumStringToInt,
            Firebelly::ComputePoints,
            {
                {ParamType::Enum, "Mawtribe", MawtribesBase::None, MawtribesBase::None, MawtribesBase::Winterbite, 1}
            },
            DESTRUCTION,
            { OGOR_MAWTRIBES }
        };
        s_registered = UnitFactory::Register("Firebelly", factoryMethod);
    }
}

Firebelly::Firebelly() :
    MawtribesBase("Firebelly", 6, WOUNDS, 6, 5, false),
    m_fireBreath(Weapon::Type::Missile, "Fire Breath", 6, 1, 0, 0, 0, 0),
    m_hammer(Weapon::Type::Melee, "Basalt Hammer", 2, 2, 3, 3, -1, RAND_D3)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, HERO, WIZARD, FIREBELLY};
    m_weapons = {&m_fireBreath, &m_hammer};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool Firebelly::configure()
{
    auto model = new Model(BASESIZE, wounds());

    model->addMissileWeapon(&m_fireBreath);
    model->addMeleeWeapon(&m_hammer);

    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    //m_knownSpells.push_back(std::make_unique<CascadingFireCloak>(this));

    m_points = Firebelly::ComputePoints(1);

    return true;
}

void Firebelly::onStartShooting(PlayerId player)
{
    MawtribesBase::onStartShooting(player);

    // Fire Breath
    if (player == owningPlayer() && m_shootingTarget)
    {
        if (distanceTo(m_shootingTarget) <= (float)m_fireBreath.range())
        {
            if (Dice::rollD6() >= 4)
            {
                m_shootingTarget->applyDamage({0, Dice::rollD3()});
            }
        }
    }
}

int Firebelly::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace OgorMawtribes
