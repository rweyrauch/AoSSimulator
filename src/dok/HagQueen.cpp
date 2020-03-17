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
bool HagQueen::s_registered = false;

HagQueen::HagQueen() :
    DaughterOfKhaine("Hag Queen", 6, WOUNDS, 8, 5, false),
    m_bladeOfKhaine(Weapon::Type::Melee, "Blade of Khaine", 1, 4, 3, 4, -1, 1)
{
    m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, PRIEST, HAG_QUEEN};
    m_weapons = {&m_bladeOfKhaine};
}

bool HagQueen::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_bladeOfKhaine);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
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
        static auto factoryMethod = new FactoryMethod{
            HagQueen::Create,
            nullptr,
            nullptr,
            HagQueen::ComputePoints,
            {
            },
            ORDER,
            { DAUGHTERS_OF_KHAINE }
        };
        s_registered = UnitFactory::Register("Hag Queen", *factoryMethod);
    }
}

} //namespace DaughtersOfKhaine

