/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/AvatarOfKhaine.h>
#include <UnitFactory.h>

namespace DaughtersOfKhaine
{
static FactoryMethod factoryMethod = {
    AvatarOfKhaine::Create,
    nullptr,
    nullptr,
    {
    },
    ORDER,
    DAUGHTERS_OF_KHAINE
};

bool AvatarOfKhaine::s_registered = false;

AvatarOfKhaine::AvatarOfKhaine() :
    DaughterOfKhaine("Avatar of Khaine", 9, WOUNDS, 10, 4, false),
    m_torrentOfBurningBlood(Weapon::Type::Missile, "Torrent of Burning Blood", 10, 6, 3, 3, -1, 1),
    m_sword(Weapon::Type::Melee, "Avatar of Khaine's Sword", 2, 4, 3, 3, -2, 3)
{
    m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, TOTEM, AVATAR_OF_KHAINE};
}

bool AvatarOfKhaine::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMissileWeapon(&m_torrentOfBurningBlood);
    model.addMeleeWeapon(&m_sword);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void AvatarOfKhaine::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_torrentOfBurningBlood);
    visitor(m_sword);
}

Unit *AvatarOfKhaine::Create(const ParameterList &parameters)
{
    auto unit = new AvatarOfKhaine();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AvatarOfKhaine::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Avatar of Khaine", factoryMethod);
    }
}

} //namespace DaughtersOfKhaine

