/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Skullgrinder.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Skullgrinder::Create,
    Skullgrinder::ValueToString,
    Skullgrinder::EnumStringToInt,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool Skullgrinder::s_registered = false;

Skullgrinder::Skullgrinder() :
    KhorneBase("Skullgrinder", 5, WOUNDS, 8, 4, false),
    m_brazenAnvil(Weapon::Type::Melee, "Brazen Anvil", 2, 3, 3, 2, -1, 3)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, SKULLGRINDER};
}

bool Skullgrinder::configure()
{
    Model model(BASESIZE, WOUNDS);
    model.addMeleeWeapon(&m_brazenAnvil);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void Skullgrinder::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_brazenAnvil);
}

Unit *Skullgrinder::Create(const ParameterList &parameters)
{
    auto unit = new Skullgrinder();

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

void Skullgrinder::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skullgrinder", factoryMethod);
    }
}

std::string Skullgrinder::ValueToString(const Parameter &parameter)
{
    return KhorneBase::ValueToString(parameter);
}

int Skullgrinder::EnumStringToInt(const std::string &enumString)
{
    return KhorneBase::EnumStringToInt(enumString);
}


} // namespace Khorne