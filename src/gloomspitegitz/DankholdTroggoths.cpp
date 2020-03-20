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

bool DankholdTroggoths::s_registered = false;

DankholdTroggoths::DankholdTroggoths() :
    GloomspiteGitzBase("Dankhold Troggoths", 6, WOUNDS, 6, 4, false),
    m_boulderClub(Weapon::Type::Melee, "Boulder Club", 2, 3, 3, 3, -2, RAND_D6)
{
    m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, DANKHOLD};
    m_weapons = {&m_boulderClub};

    s_globalBraveryMod.connect(this, &DankholdTroggoths::reassuringPresence, &m_connection);
}

DankholdTroggoths::~DankholdTroggoths()
{
    m_connection.disconnect();
}

bool DankholdTroggoths::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_boulderClub);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

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
        static FactoryMethod factoryMethod = {
            Create,
            nullptr,
            nullptr,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
            },
            DESTRUCTION,
            { GLOOMSPITE_GITZ }
        };

        s_registered = UnitFactory::Register("Dankhold Troggoths", factoryMethod);
    }
}

void DankholdTroggoths::onStartHero(PlayerId player)
{
    if (player == owningPlayer())
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            // Regeneration - heal D3
            // Troggoth Renewal
            if (Dice::rollD6() >= 4 || (inLightOfTheBadMoon() && (Dice::rollD6() >= 4)))
            {
                int woundsHealed = Dice::rollD3();
                if (inLightOfTheBadMoon())
                    woundsHealed *= 2;
                for (auto &m : m_models)
                {
                    if (!m->slain() || !m->fled())
                    {
                        if (m->woundsRemaining() < wounds())
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
    if (m_meleeTarget)
    {
        // Crushing Grip
        int roll = Dice::rollD6();
        if (roll >= m_meleeTarget->wounds())
        {
            m_meleeTarget->slay(1);
        }
    }

    // Squiggly-beast Followers
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    for (auto ip : units)
    {
        int roll = Dice::rollD6();
        if (roll >= ip->remainingModels())
        {
            ip->applyDamage({0, 1});
        }
    }
    GloomspiteGitzBase::onStartCombat(player);
}

int DankholdTroggoths::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int DankholdTroggoths::reassuringPresence(const Unit *unit)
{
    // Reassuring Presence
    if (unit->hasKeyword(GLOOMSPITE_GITZ) && (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 18.0f))
    {
        return 1;
    }

    return 0;
}

} // namespace GloomspiteGitz
