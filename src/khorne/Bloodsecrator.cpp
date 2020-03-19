/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodsecrator.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{

bool Bloodsecrator::s_registered = false;

Bloodsecrator::Bloodsecrator() :
    KhorneBase("Bloodsecrator", 4, WOUNDS, 9, 3, false),
    m_ensorcelledAxe(Weapon::Type::Melee, "Ensorcelled Axe", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, TOTEM, BLOODSECRATOR};
    m_weapons = {&m_ensorcelledAxe};
}

bool Bloodsecrator::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_ensorcelledAxe);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Bloodsecrator::Create(const ParameterList &parameters)
{
    auto unit = new Bloodsecrator();

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bloodsecrator::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Bloodsecrator::Create,
            KhorneBase::ValueToString,
            KhorneBase::EnumStringToInt,
            Bloodsecrator::ComputePoints,
            {
                {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
            },
            CHAOS,
            { KHORNE }
        };
        s_registered = UnitFactory::Register("Bloodsecrator", factoryMethod);
    }
}

} // namespace Khorne