/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/DankholdTroggoths.h>
#include <UnitFactory.h>
#include <Board.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    DankholdTroggoths::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", DankholdTroggoths::MIN_UNIT_SIZE, DankholdTroggoths::MIN_UNIT_SIZE,
            DankholdTroggoths::MAX_UNIT_SIZE, DankholdTroggoths::MIN_UNIT_SIZE
        },
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool DankholdTroggoths::s_registered = false;

DankholdTroggoths::DankholdTroggoths() :
    GloomspiteGitzBase("Dankhold Troggoths", 6, WOUNDS, 6, 4, false),
    m_boulderClub(Weapon::Type::Melee, "Boulder Club", 2, 3, 3, 3, -2, RAND_D6)
{
    m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, DANKHOLD};
    m_weapons = {&m_boulderClub};
}

bool DankholdTroggoths::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_boulderClub);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *DankholdTroggoths::Create(const ParameterList &parameters)
{
    auto unit = new DankholdTroggoths();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DankholdTroggoths::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dankhold Troggoths", factoryMethod);
    }
}

void DankholdTroggoths::onStartHero(PlayerId player)
{
    if (player == m_owningPlayer)
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            Dice dice;
            // Regeneration - heal D3
            // Troggoth Renewal
            if (dice.rollD6() >= 4 || (inLightOfTheBadMoon() && (dice.rollD6() >= 4)))
            {
                int woundsHealed = dice.rollD3();
                if (inLightOfTheBadMoon())
                    woundsHealed *= 2;
                for (auto &m : m_models)
                {
                    if (!m->slain() || !m->fled())
                    {
                        if (m->woundsRemaining() < WOUNDS)
                        {
                            int numToHeal = std::min(woundsHealed, WOUNDS - m->woundsRemaining());
                            m->applyHealing(numToHeal);
                            woundsHealed -= numToHeal;
                            if (woundsHealed <= 0) { break; }
                        }
                    }
                }
            }
        }
    }
}

void DankholdTroggoths::onStartCombat(PlayerId player)
{
    Dice dice;
    if (m_meleeTarget)
    {
        // Crushing Grip
        int roll = dice.rollD6();
        if (roll >= m_meleeTarget->wounds())
        {
            m_meleeTarget->slay(1);
        }
    }

    // Squiggly-beast Followers
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
    for (auto ip : units)
    {
        int roll = dice.rollD6();
        if (roll >= ip->remainingModels())
        {
            ip->applyDamage({0, 1});
        }
    }
    GloomspiteGitzBase::onStartCombat(player);
}

} // namespace GloomspiteGitz
