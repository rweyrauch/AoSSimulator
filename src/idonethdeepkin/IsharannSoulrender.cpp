/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "idonethdeepkin/IsharannSoulrender.h"

namespace IdonethDeepkin
{

bool IsharannSoulrender::s_registered = false;

Unit *IsharannSoulrender::Create(const ParameterList &parameters)
{
    auto unit = new IsharannSoulrender();

    auto enclave = (Enclave)GetEnumParam("Enclave", parameters, Enclave::None);
    unit->setEnclave(enclave);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void IsharannSoulrender::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            IdonethDeepkinBase::ValueToString,
            IdonethDeepkinBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Enclave", IdonethDeepkinBase::None, IdonethDeepkinBase::None, IdonethDeepkinBase::Briomdar, 1},
            },
            ORDER,
            { IDONETH_DEEPKIN }
        };

        s_registered = UnitFactory::Register("Isharann Soulrender", *factoryMethod);
    }
}

IsharannSoulrender::IsharannSoulrender() :
    IdonethDeepkinBase("Isharann Soulrender", 6, WOUNDS, 8, 4, false),
    m_talunhook(Weapon::Type::Melee, "Talunhook", 2, 2, 3, 3, -1, 2),
    m_bill(Weapon::Type::Melee, "Rakerdart's Serrated Bill", 3, RAND_D3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, ISHARANN, HERO, SOULRENDER};
    m_weapons = {&m_talunhook, &m_bill};
}

bool IsharannSoulrender::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_talunhook);
    model->addMeleeWeapon(&m_bill);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //IdonethDeepkin