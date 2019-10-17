/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodstoker.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Bloodstoker::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool Bloodstoker::s_registered = false;

Bloodstoker::Bloodstoker() :
    KhorneBase("Bloodstoker", 6, WOUNDS, 8, 4, false),
    m_tortureBlade(Weapon::Type::Melee, "Torture Blade", 1, 3, 3, 3, 0, 1),
    m_bloodWhip(Weapon::Type::Melee, "Blood Whip", 3, 3, 3, 4, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, BLOODSTOKER};
}

bool Bloodstoker::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_tortureBlade);
    model.addMeleeWeapon(&m_bloodWhip);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Bloodstoker::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_tortureBlade);
    visitor(m_bloodWhip);
}

Unit *Bloodstoker::Create(const ParameterList &parameters)
{
    auto unit = new Bloodstoker();

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

void Bloodstoker::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodstoker", factoryMethod);
    }
}

} // namespace Khorne