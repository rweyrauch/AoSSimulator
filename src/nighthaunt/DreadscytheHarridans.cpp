/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/DreadscytheHarridans.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt
{

static FactoryMethod factoryMethod = {
    DreadscytheHarridans::Create,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = DreadscytheHarridans::MIN_UNIT_SIZE}, DreadscytheHarridans::MIN_UNIT_SIZE,
         DreadscytheHarridans::MAX_UNIT_SIZE, DreadscytheHarridans::MIN_UNIT_SIZE},
    }
};

Weapon DreadscytheHarridans::s_scythedLimbs(Weapon::Type::Melee, "Scythed Limbs", 1, 3, 4, 3, -1, 1);
Weapon DreadscytheHarridans::s_scythedLimbsCrone(Weapon::Type::Melee, "Scythed Limbs (Crone)", 1, 4, 4, 3, -1, 1);

bool DreadscytheHarridans::s_registered = false;

DreadscytheHarridans::DreadscytheHarridans() :
    Unit("Dreadscythe Harridans", 8, WOUNDS, 10, 4, true)
{
    m_keywords = { DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, DREADSCYTHE_HARRIDANS };
}

bool DreadscytheHarridans::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
        return false;

    Model crone(BASESIZE, WOUNDS);
    crone.addMeleeWeapon(&s_scythedLimbsCrone);
    addModel(crone);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_scythedLimbs);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_scythedLimbs.name() << ": " << s_scythedLimbs.strength() << std::endl;
        std::cout << "\t" << s_scythedLimbsCrone.name() << ": " << s_scythedLimbsCrone.strength() << std::endl;
    }

    return true;
}

Unit *DreadscytheHarridans::Create(const ParameterList &parameters)
{
    auto unit = new DreadscytheHarridans();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void DreadscytheHarridans::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dreadscythe Harridans", factoryMethod);
    }
}

int DreadscytheHarridans::toSaveModifier(const Weapon *weapon) const
{
    // Etherial - no save modifiers allowed.
    int modifier = 0;

    // Etherial - ignore rend by cancelling it out.
    if (weapon->rend() < 0)
        modifier = -weapon->rend();

    return modifier;
}

} // namespace Nighthaunt
