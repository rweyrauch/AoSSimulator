/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Dryads.h>
#include <UnitFactory.h>
#include <iostream>

namespace Sylvaneth {

static FactoryMethod factoryMethod = {
    Dryads::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = Dryads::MIN_UNIT_SIZE}, Dryads::MIN_UNIT_SIZE,
         Dryads::MAX_UNIT_SIZE, Dryads::MIN_UNIT_SIZE},
    }
};

bool Dryads::s_registered = false;

Weapon Dryads::s_wrackingTalons(Weapon::Type::Melee, "Wracking Talons", 2, 2, 4, 4, 0, 1);
Weapon Dryads::s_wrackingTalonsNymph(Weapon::Type::Melee, "Wracking Talons (Nymph)", 2, 3, 4, 4, 0, 1);

Dryads::Dryads() :
    Unit("Dryads", 7, WOUNDS, 6, 5, false)
{
    m_keywords = { ORDER, SYLVANETH, DRYADS };
}

bool Dryads::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model nymph(BASESIZE, WOUNDS);
    nymph.addMeleeWeapon(&s_wrackingTalonsNymph);
    addModel(nymph);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_wrackingTalons);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_wrackingTalons.name() << ": " << s_wrackingTalons.strength() << std::endl;
        std::cout << "\t" << s_wrackingTalonsNymph.name() << ": " << s_wrackingTalonsNymph.strength() << std::endl;
    }

    return true;
}

Unit *Dryads::Create(const ParameterList &parameters)
{
    auto unit = new Dryads();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Dryads::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Dryads", factoryMethod);
    }
}

} // namespace Sylvaneth