/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <idonethdeepkin/Volturnos.h>
#include <UnitFactory.h>
#include <Board.h>

namespace IdonethDeepkin
{
static FactoryMethod factoryMethod = {
    Volturnos::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    { IDONETH_DEEPKIN }
};

bool Volturnos::s_registered = false;

Volturnos::Volturnos() :
    Unit("Volturnos", 14, WOUNDS, 8, 3, true),
    m_theAstraSolus(Weapon::Type::Melee, "The Astra Solus", 1, 5, 3, 3, -1, RAND_D3),
    m_deepmareJawsTalons(Weapon::Type::Melee, "Deepmare's Fanged Jaw and Talons", 2, 3, 3, 3, -1, 1),
    m_deepmareTails(Weapon::Type::Melee, "Deepmare's Lashing Tails", 2, 3, 3, 3, 0, 2)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, HERO, AKHELIAN, AKHELIAN_KING, VOLTURNOS};
    m_weapons = {&m_theAstraSolus, &m_deepmareJawsTalons, &m_deepmareTails};
}

bool Volturnos::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_theAstraSolus);
    model->addMeleeWeapon(&m_deepmareJawsTalons);
    model->addMeleeWeapon(&m_deepmareTails);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Volturnos::Create(const ParameterList &parameters)
{
    auto unit = new Volturnos();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Volturnos::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Volturnos", factoryMethod);
    }
}

void Volturnos::onCharged()
{
    // Deepmare Horn
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1.0f);
    if (!units.empty())
    {
        Dice dice;
        int roll = dice.rollD6();
        if (roll >= 2)
        {
            units.front()->applyDamage({0, dice.rollD3()});
        }
    }

    Unit::onCharged();
}

} //namespace IdonethDeepkin
