/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/BoingrotBounderz.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    BoingrotBounderz::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = BoingrotBounderz::MIN_UNIT_SIZE},
         BoingrotBounderz::MIN_UNIT_SIZE, BoingrotBounderz::MAX_UNIT_SIZE, BoingrotBounderz::MIN_UNIT_SIZE}
    }
};

bool BoingrotBounderz::s_registered = false;

BoingrotBounderz::BoingrotBounderz() :
    Unit("Boingrot Bounderz", RAND_2D6, WOUNDS, 5, 4, true),
    m_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1),
    m_pokinLance(Weapon::Type::Melee, "Pokin' Lance", 2, 2, 4, 4, -1, 1),
    m_pokinLanceBoss(Weapon::Type::Melee, "Pokin' Lance (Boss)", 2, 2, 3, 4, -1, 1)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, BOINGROT_BOUNDERZ };
}

bool BoingrotBounderz::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the boss
    Model boss(BASESIZE, WOUNDS);
    boss.addMeleeWeapon(&m_pokinLanceBoss);
    boss.addMeleeWeapon(&m_fangFilledGob);
    addModel(boss);

    // and the rest
    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_pokinLance);
        model.addMeleeWeapon(&m_fangFilledGob);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

Unit *BoingrotBounderz::Create(const ParameterList &parameters)
{
    auto unit = new BoingrotBounderz();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BoingrotBounderz::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Boingrot Bounderz", factoryMethod);
    }
}

void BoingrotBounderz::onCharged()
{
    if (m_meleeTarget)
    {
        // Boing! Smash!
        auto dist = distanceTo(m_meleeTarget);
        if (dist <= 1)
        {
            Dice dice;
            int numEnemyModels = m_meleeTarget->remainingModels();

            Dice::RollResult rolls;
            dice.rollD6(numEnemyModels, rolls);
            int numMortalWounds = rolls.rollsGE(4);

            m_meleeTarget->applyDamage({0, numMortalWounds});
        }
    }
    Unit::onCharged();
}

int BoingrotBounderz::toWoundModifier(const Weapon *weapon, const Unit *unit) const
{
    // Lances of the Bounderz
    int modifier = Unit::toWoundModifier(weapon, unit);
    if (m_charged && weapon->name() == m_pokinLance.name())
        modifier += 1;

    return modifier;
}

void BoingrotBounderz::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_pokinLance);
    visitor(&m_pokinLanceBoss);
    visitor(&m_fangFilledGob);
}

} // namespace GloomspiteGitz
