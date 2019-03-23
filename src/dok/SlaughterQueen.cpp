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
static FactoryMethod factoryMethod = {
    SlaughterQueen::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    DAUGHTERS_OF_KHAINE
};

bool SlaughterQueen::s_registered = false;

SlaughterQueen::SlaughterQueen() :
    DaughterOfKhaine("Slaughter Queen", 6, WOUNDS, 9, 5, false),
    m_bladeOfKhaine(Weapon::Type::Melee, "Blade of Khaine", 1, 4, 3, 4, -1, 1),
    m_deathsword(Weapon::Type::Melee, "Deathsword", 1, 3, 3, 3, -1, RAND_D3)
{
    m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, PRIEST, SLAUGHTER_QUEEN};
}

bool SlaughterQueen::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_bladeOfKhaine);
    model.addMeleeWeapon(&m_deathsword);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void SlaughterQueen::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bladeOfKhaine);
    visitor(&m_deathsword);
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
        s_registered = UnitFactory::Register("Slaughter Queen", factoryMethod);
    }
}

} //namespace DaughtersOfKhaine
