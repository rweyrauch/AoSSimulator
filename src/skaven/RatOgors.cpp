/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/RatOgors.h>
#include <UnitFactory.h>

namespace Skaven
{
bool RatOgors::s_registered = false;

Unit *RatOgors::Create(const ParameterList &parameters)
{
    auto unit = new RatOgors();
    int numModels = 0;

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int RatOgors::ComputePoints(int numModels)
{
    return 0;
}

void RatOgors::Init()
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

        s_registered = UnitFactory::Register("Rat Ogors", *factoryMethod);
    }
}

RatOgors::RatOgors() :
    Skaventide("Rat Ogors", 6, WOUNDS, 5, 5, false),
    m_gun(Weapon::Type::Missile, "Warpfire Gun", 16, 1, 5, 3, -1, RAND_D3),
    m_clawsBladesAndFangs(Weapon::Type::Melee, "Tearing Claws, Blades and Fangs", 1, 4, 4, 3, -1, 2)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_MOULDER, FIGHTING_BEAST, PACK, RAT_OGORS};
    m_weapons = {&m_gun, &m_clawsBladesAndFangs};
}

bool RatOgors::configure(int numModels)
{
    return false;
}
} //namespace Skaven
