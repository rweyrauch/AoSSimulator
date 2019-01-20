/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/ChainraspHorde.h>
#include <UnitFactory.h>

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    ChainraspHorde::Create,
    {
        {ParamType::Integer, "numModels", {.m_intValue = ChainraspHorde::MIN_UNIT_SIZE}, ChainraspHorde::MIN_UNIT_SIZE,
         ChainraspHorde::MAX_UNIT_SIZE},
    }
};

Weapon ChainraspHorde::s_malignantWeapon(Weapon::Type::Melee, "Malignant Weapon", 1, 2, 4, 4, 0, 1);
Weapon ChainraspHorde::s_malignantWeaponWarden(Weapon::Type::Melee, "Malignant Weapon", 1, 3, 4, 4, 0, 1);

bool ChainraspHorde::s_registered = false;

ChainraspHorde::ChainraspHorde() :
    Unit("Chainrasp Horde", 6, WOUNDS, 10, 5, true) // todo: bravery 6 when no Dreadwarden
{
    m_keywords = { DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, CHAINRASP_HORDE };
}

bool ChainraspHorde::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
        return false;

    Model warden(BASESIZE, WOUNDS);
    warden.addMeleeWeapon(&s_malignantWeaponWarden);
    addModel(warden);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_malignantWeapon);
        addModel(model);
    }

    return true;
}

Unit *ChainraspHorde::Create(const ParameterList &parameters)
{
    auto unit = new ChainraspHorde();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void ChainraspHorde::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Chainrasp Horde", factoryMethod);
    }
}

Rerolls ChainraspHorde::toWoundRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Chilling Horde
    if (remainingModels() >= 10)
        return RerollOnes;

    return Unit::toHitRerolls(weapon, unit);
}

int ChainraspHorde::toSaveModifier(const Weapon *weapon) const
{
    // Etherial - no save modifiers allowed.
    int modifier = 0;

    // Etherial - ignore rend by cancelling it out.
    if (weapon->rend() < 0)
        modifier = -weapon->rend();

    return modifier;
}

} // namespace Nighthaunt