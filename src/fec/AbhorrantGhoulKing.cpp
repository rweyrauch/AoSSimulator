/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/AbhorrantGhoulKing.h>
#include <UnitFactory.h>

namespace FleshEaterCourt
{
static FactoryMethod factoryMethod = {
    AbhorrantGhoulKing::Create,
    nullptr,
    nullptr,
    {
    },
    DEATH,
    FLESH_EATER_COURTS
};

bool AbhorrantGhoulKing::s_registered = false;

AbhorrantGhoulKing::AbhorrantGhoulKing() :
    FleshEaterCourts("Abhorrant Ghoul King", 6, WOUNDS, 10, 4, false),
    m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 6, 3, 3, -1, 1)
{
    m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, HERO, WIZARD,
        ABHORRANT_GHOUL_KING};
}

bool AbhorrantGhoulKing::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_goryTalonsAndFangs);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void AbhorrantGhoulKing::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_goryTalonsAndFangs);
}

Unit *AbhorrantGhoulKing::Create(const ParameterList &parameters)
{
    auto unit = new AbhorrantGhoulKing();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AbhorrantGhoulKing::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Abhorrant Ghoul King", factoryMethod);
    }
}

void AbhorrantGhoulKing::hero(PlayerId player)
{
    Unit::hero(player);

    // Royal Blood
    if (player == m_owningPlayer)
    {
        if (remainingWounds() < WOUNDS && remainingWounds() > 0)
        {
            Dice dice;
            int woundsHealed = dice.rollD3();
            for (auto &m : m_models)
            {
                m.woundsRemaining() += woundsHealed;
                if (m.woundsRemaining() > WOUNDS)
                {
                    m.woundsRemaining() = WOUNDS;
                }
            }
        }
    }
}

} // namespace FleshEasterCourt
