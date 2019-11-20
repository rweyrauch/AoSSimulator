/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Ironguts.h"

namespace OgorMawtribes
{
static FactoryMethod factoryMethod = {
    Ironguts::Create,
    Ironguts::ValueToString,
    Ironguts::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Ironguts::MIN_UNIT_SIZE, Ironguts::MIN_UNIT_SIZE,
            Ironguts::MAX_UNIT_SIZE, Ironguts::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Rune Maw Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Bellower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    DESTRUCTION,
    OGOR_MAWTRIBES
};

bool Ironguts::s_registered = false;

Unit *Ironguts::Create(const ParameterList &parameters)
{
    auto unit = new Ironguts();

    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool mawBearer = GetBoolParam("Rune Maw Bearer", parameters, true);
    bool bellower = GetBoolParam("Bellower", parameters, true);

    bool ok = unit->configure(numModels, mawBearer, bellower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Ironguts::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ironguts", factoryMethod);
    }
}

Ironguts::Ironguts() :
    MawtribesBase("Ironguts", 6, WOUNDS, 7, 4, false),
    m_bashingWeapon(Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 3, 3, 3, 0, 2),
    m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1),
    m_bashingWeaponGutlord(Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 4, 3, 3, 0, 2)
{
    m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, IRONGUTS};
}

bool Ironguts::configure(int numModels, bool runeMawBearer, bool bellower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_runeMawBearer = runeMawBearer;
    m_bellower = bellower;

    Model gutlord(BASESIZE, WOUNDS);
    gutlord.addMeleeWeapon(&m_bashingWeaponGutlord);
    gutlord.addMeleeWeapon(&m_bite);
    addModel(gutlord);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_bashingWeapon);
        model.addMeleeWeapon(&m_bite);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Ironguts::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_bashingWeapon);
    visitor(m_bite);
    visitor(m_bashingWeaponGutlord);
}

} // namespace OgorMawtribes
