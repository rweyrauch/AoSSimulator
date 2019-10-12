/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/WardenKing.h>
#include <UnitFactory.h>

namespace Dispossessed
{
static FactoryMethod factoryMethod = {
    WardenKing::Create,
    Dispossessed::ValueToString,
    Dispossessed::EnumStringToInt,
    {
        {ParamType::Enum, "Grudge", Dispossessed::StuckUp, Dispossessed::StuckUp, Dispossessed::SneakyAmbushers, 1}
    },
    ORDER,
    DISPOSSESSED
};

bool WardenKing::s_registered = false;

WardenKing::WardenKing() :
    Dispossessed("Warden King", 4, WOUNDS, 8, 4, false),
    m_runeWeapon(Weapon::Type::Melee, "Rune Weapon", 1, 4, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HERO, WARDEN_KING};
}

bool WardenKing::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_runeWeapon);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void WardenKing::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_runeWeapon);
}

Unit *WardenKing::Create(const ParameterList &parameters)
{
    auto unit = new WardenKing();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void WardenKing::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Warden King", factoryMethod);
    }
}

} // namespace Dispossessed