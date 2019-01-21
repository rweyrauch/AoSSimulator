/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/BladegheistRevenants.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    BladegheistRevenants::Create,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = BladegheistRevenants::MIN_UNIT_SIZE}, BladegheistRevenants::MIN_UNIT_SIZE,
         BladegheistRevenants::MAX_UNIT_SIZE, BladegheistRevenants::MIN_UNIT_SIZE},
    }
};

Weapon BladegheistRevenants::s_tombGreatblade(Weapon::Type::Melee, "Tomb Greatblade", 1, 2, 3, 3, -1, 1);

bool BladegheistRevenants::s_registered = false;

BladegheistRevenants::BladegheistRevenants() :
    Unit("Bladegheist Revenants", 8, WOUNDS, 10, 4, true)
{
    m_keywords = { DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, BLADEGHEIST_REVENANTS };
}

bool BladegheistRevenants::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
        return false;

    m_runAndCharge = true;

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_tombGreatblade);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_tombGreatblade.name() << ": " << s_tombGreatblade.strength() << std::endl;
    }

    return true;
}

Unit *BladegheistRevenants::Create(const ParameterList &parameters)
{
    auto unit = new BladegheistRevenants();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BladegheistRevenants::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bladegheist Revenants", factoryMethod);
    }
}

Rerolls BladegheistRevenants::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Fearful Frenzy
    if (false) // wholly w/in 12" of friendly 'SPIRIT_TORMENTS' or 'CHAINGHASTS'
        return RerollFailed;

    return Unit::toHitRerolls(weapon, unit);
}

int BladegheistRevenants::toSaveModifier(const Weapon *weapon) const
{
    // Etherial - no save modifiers allowed.
    int modifier = 0;

    // Etherial - ignore rend by cancelling it out.
    if (weapon->rend() < 0)
        modifier = -weapon->rend();

    return modifier;
}

int BladegheistRevenants::extraAttacks(const Weapon *weapon) const
{
    // Whirling Death
    int attacks = Unit::extraAttacks(weapon);
    if (m_charged)
        attacks += 1;
    return attacks;
}

} // namespace Nighthaunt
