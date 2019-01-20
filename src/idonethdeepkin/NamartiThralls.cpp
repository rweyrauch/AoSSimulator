/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/NamartiThralls.h>
#include <UnitFactory.h>

namespace IdonethDeepkin {

static FactoryMethod factoryMethod = {
    NamartiThralls::Create,
    {
        {ParamType::Integer, "numModels", {.m_intValue = NamartiThralls::MIN_UNIT_SIZE}, NamartiThralls::MIN_UNIT_SIZE,
         NamartiThralls::MAX_UNIT_SIZE},
        {ParamType::Integer, "numIconBearers", {.m_intValue = 0}, 0, NamartiThralls::MAX_UNIT_SIZE/10}
    }
};

Weapon NamartiThralls::s_lanmariBlade(Weapon::Type::Melee, "Lanmari Blade", 1, 2, 3, 3, -1, 1);

bool NamartiThralls::s_registered = false;

NamartiThralls::NamartiThralls() :
    Unit("Namarti Thralls", 6, WOUNDS, 6, 5, false)
{
    m_keywords = { ORDER, AELF, IDONETH_DEEPKIN, NAMARTI, THRALLS };
}

bool NamartiThralls::configure(int numModels, int numIconBearers)
{
    return false;
}

Unit *NamartiThralls::Create(const ParameterList &parameters)
{
    return nullptr;
}

void NamartiThralls::Init()
{

}

} // namespace IdonethDeepkin