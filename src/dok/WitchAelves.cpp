/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/WitchAelves.h>
#include <UnitFactory.h>

namespace DaughtersOfKhaine {

static FactoryMethod factoryMethod = {
    WitchAelves::Create,
    {
        {ParamType::Integer, "numModels", {.m_intValue = WitchAelves::MIN_UNIT_SIZE}, WitchAelves::MIN_UNIT_SIZE,
         WitchAelves::MAX_UNIT_SIZE},
        {ParamType::Boolean, "pairedKnives", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = false}, false, false},
        {ParamType::Boolean, "standardBearers", {.m_boolValue = false}, false, false},
    }
};

Weapon WitchAelves::s_sacrificialKnife(Weapon::Type::Melee, "Sacrificial Knife", 1, 2, 3, 4, 0, 1);
Weapon WitchAelves::s_sacrificialKnifeHag(Weapon::Type::Melee, "Sacrificial Knife", 1, 2, 2, 4, 0, 1);

bool WitchAelves::s_registered = false;

WitchAelves::WitchAelves() :
    Unit("Witch Aelves", 6, WOUNDS, 7, 6, false)
{
    m_keywords = { ORDER, AELF, DAUGHTERS_OF_KHAINE, WITCH_AELVES };
}

bool WitchAelves::configure(int numModels, bool pairedKnives, bool hornblowers, bool standardBearers)
{
    return false;
}

Unit *WitchAelves::Create(const ParameterList &parameters)
{
    return nullptr;
}

void WitchAelves::Init()
{

}

} // namespace DaughtersOfKhaine