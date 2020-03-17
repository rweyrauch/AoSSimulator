/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/SlaughterQueen.h>
#include <UnitFactory.h>

namespace DaughtersOfKhaine
{
bool SlaughterQueen::s_registered = false;

SlaughterQueen::SlaughterQueen() :
    DaughterOfKhaine("Slaughter Queen", 6, WOUNDS, 9, 5, false),
    m_bladeOfKhaine(Weapon::Type::Melee, "Blade of Khaine", 1, 4, 3, 4, -1, 1),
    m_deathsword(Weapon::Type::Melee, "Deathsword", 1, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, PRIEST, SLAUGHTER_QUEEN};
    m_weapons = {&m_bladeOfKhaine, &m_deathsword};
}

bool SlaughterQueen::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_bladeOfKhaine);
    model->addMeleeWeapon(&m_deathsword);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *SlaughterQueen::Create(const ParameterList &parameters)
{
    auto unit = new SlaughterQueen();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SlaughterQueen::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            SlaughterQueen::Create,
            nullptr,
            nullptr,
            SlaughterQueen::ComputePoints,
            {
            },
            ORDER,
            { DAUGHTERS_OF_KHAINE }
        };
        s_registered = UnitFactory::Register("Slaughter Queen", *factoryMethod);
    }
}

} //namespace DaughtersOfKhaine

