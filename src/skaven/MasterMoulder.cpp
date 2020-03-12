/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/MasterMoulder.h>
#include <UnitFactory.h>

namespace Skaven
{
bool MasterMoulder::s_registered = false;

Unit *MasterMoulder::Create(const ParameterList &parameters)
{
    auto unit = new MasterMoulder();

    WeaponOption option = Lash;

    bool ok = unit->configure(option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void MasterMoulder::Init()
{
    if (!s_registered)
    {
        static auto factoryMethod = new FactoryMethod{
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Master Moulder", *factoryMethod);
    }
}

MasterMoulder::MasterMoulder() :
    Skaventide("Master Moulder", 6, WOUNDS, 6, 5, false),
    m_lash(Weapon::Type::Melee, "Warpstone-tipped Lash", 3, 6, 3, 4, -1, 1),
    m_catcher(Weapon::Type::Melee, "Things-catcher", 2, 4, 4, 4, -1, 2)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_MOULDER, HERO, MASTER_MOULDER};
    m_weapons = {&m_lash, &m_catcher};
}

bool MasterMoulder::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, WOUNDS);
    if (option == Lash)
        model->addMeleeWeapon(&m_lash);
    else if (option == ThingsCatcher)
        model->addMeleeWeapon(&m_catcher);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //namespace Skaven
