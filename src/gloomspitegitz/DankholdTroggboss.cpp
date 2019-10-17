/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/DankholdTroggboss.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    DankholdTroggboss::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool DankholdTroggboss::s_registered = false;

DankholdTroggboss::DankholdTroggboss() :
    GloomspiteGitzBase("Dankhold Troggboss", 6, WOUNDS, 7, 4, false),
    m_boulderClub(Weapon::Type::Melee, "Boulder Club", 2, 4, 3, 3, -2, RAND_D6)
{
    m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, DANKHOLD, HERO, TROGGBOSS};
}

bool DankholdTroggboss::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_boulderClub);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void DankholdTroggboss::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_boulderClub);
}

Unit *DankholdTroggboss::Create(const ParameterList &parameters)
{
    auto unit = new DankholdTroggboss();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DankholdTroggboss::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dankhold Troggboss", factoryMethod);
    }
}

void DankholdTroggboss::onStartHero(PlayerId player)
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
                    m.applyHealing(woundsHealed);
                }
            }
        }
    }
}

void DankholdTroggboss::onStartCombat(PlayerId player)
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

