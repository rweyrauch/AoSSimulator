/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightAzyros.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    KnightAzyros::Create,
    StormcastEternal::ValueToString,
    StormcastEternal::EnumStringToInt,
    {
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    { STORMCAST_ETERNAL }
};

bool KnightAzyros::s_registered = false;

KnightAzyros::KnightAzyros() :
    StormcastEternal("Knight-Azyros", 12, WOUNDS, 9, 3, true),
    m_starblade(Weapon::Type::Melee, "Starblade", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_AZYROS};
    m_weapons = {&m_starblade};
}

bool KnightAzyros::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_starblade);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *KnightAzyros::Create(const ParameterList &parameters)
{
    auto unit = new KnightAzyros();

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KnightAzyros::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Knight-Azyros", factoryMethod);
    }
}

void KnightAzyros::onStartHero(PlayerId player)
{
    if ((player == m_owningPlayer) && !m_usedLightOfSigmar)
    {
        // The Light of Sigmar - use the first chance is can
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 8.0f);
        if (!units.empty())
        {
            Dice dice;

            for (auto ip : units)
            {
                if (ip->hasKeyword(CHAOS))
                {
                    ip->applyDamage({0, dice.rollD6()});
                }
                else
                {
                    ip->applyDamage({0, dice.rollD3()});
                }
            }
            m_usedLightOfSigmar = true;
        }
    }
    Unit::onStartHero(player);
}


} // namespace StormcastEternals