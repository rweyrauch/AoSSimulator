/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/HagQueen.h>
#include <UnitFactory.h>

namespace DaughtersOfKhaine
{
static FactoryMethod factoryMethod = {
    HagQueen::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    DAUGHTERS_OF_KHAINE
};

bool HagQueen::s_registered = false;

HagQueen::HagQueen() :
    DaughterOfKhaine("Hag Queen", 6, WOUNDS, 8, 5, false),
    m_bladeOfKhaine(Weapon::Type::Melee, "Blade of Khaine", 1, 4, 3, 4, -1, 1)
{
    m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, PRIEST, HAG_QUEEN};
}

bool HagQueen::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_bladeOfKhaine);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void HagQueen::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bladeOfKhaine);
}

Unit *HagQueen::Create(const ParameterList &parameters)
{
    auto unit = new HagQueen();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void HagQueen::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Hag Queen", factoryMethod);
    }
}

} //namespace DaughtersOfKhaine
