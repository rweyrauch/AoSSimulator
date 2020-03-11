/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "idonethdeepkin/IsharannTidecaster.h"

namespace IdonethDeepkin
{

bool IsharannTidecaster::s_registered = false;

Unit *IsharannTidecaster::Create(const ParameterList &parameters)
{
    auto unit = new IsharannTidecaster();

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

void IsharannTidecaster::Init()
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

        s_registered = UnitFactory::Register("Isharann Tidecaster", *factoryMethod);
    }
}

IsharannTidecaster::IsharannTidecaster() :
    IdonethDeepkinBase("Isharann Tidecaster", 6, WOUNDS, 7, 6, false),
    m_staff(Weapon::Type::Melee, "Pelagic Staff", 1, 2, 3, 3, 0, RAND_D3)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, ISHARANN, HERO, WIZARD, TIDECASTER};
    m_weapons = {&m_staff};

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

bool IsharannTidecaster::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_staff);

    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

} //IdonethDeepkin