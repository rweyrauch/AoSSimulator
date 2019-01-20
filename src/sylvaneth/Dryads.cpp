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

namespace Sylvaneth {

static FactoryMethod factoryMethod = {
    Dryads::Create,
    {
        {ParamType::Integer, "numModels", {.m_intValue = Dryads::MIN_UNIT_SIZE}, Dryads::MIN_UNIT_SIZE,
         Dryads::MAX_UNIT_SIZE},
    }
};

bool Dryads::s_registered = false;

Weapon Dryads::s_wrackingTalons(Weapon::Type::Melee, "Wracking Talons", 2, 2, 4, 4, 0, 1);
Weapon Dryads::s_wrackingTalonsNymph(Weapon::Type::Melee, "Wracking Talons", 2, 3, 4, 4, 0, 1);

Dryads::Dryads() :
    Unit("Dryads", 7, WOUNDS, 6, 5, false)
{
    m_keywords = { ORDER, SYLVANETH, DRYADS };
}

bool Dryads::configure(int numModels)
{
    return false;
}

Unit *Dryads::Create(const ParameterList &parameters)
{
    return nullptr;
}

void Dryads::Init()
{

}

} // namespace Sylvaneth