/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GiantRats.h>
#include <UnitFactory.h>

namespace Skaven
{
bool GiantRats::s_registered = false;

Unit *GiantRats::Create(const ParameterList &parameters)
{
    auto unit = new GiantRats();
    int models = 0;

    bool ok = unit->configure(models);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int GiantRats::ComputePoints(int numModels)
{
    return 0;
}

void GiantRats::Init()
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

        s_registered = UnitFactory::Register("Giant Rats", *factoryMethod);
    }
}

GiantRats::GiantRats() :
    Skaventide("Giant Rats", 8, WOUNDS, 3, NoSave, false),
    m_teeth(Weapon::Type::Melee, "Vicious Teeth", 1, 1, 4, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVENTIDE, CLANS_MOULDER, PACK, GIANT_RATS};
    m_weapons = {&m_teeth};
}

bool GiantRats::configure(int numModels)
{
    return false;
}
} //namespace Skaven
