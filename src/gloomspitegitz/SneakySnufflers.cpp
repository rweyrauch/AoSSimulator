/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/SneakySnufflers.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include <Roster.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
    SneakySnufflers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", SneakySnufflers::MIN_UNIT_SIZE, SneakySnufflers::MIN_UNIT_SIZE,
            SneakySnufflers::MAX_UNIT_SIZE, SneakySnufflers::MIN_UNIT_SIZE
        }
    },
    DESTRUCTION,
    GLOOMSPITE_GITZ
};

bool SneakySnufflers::s_registered = false;

SneakySnufflers::SneakySnufflers() :
    GloomspiteGitzBase("Sneaky Snufflers", 5, WOUNDS, 4, 6, false),
    m_sickle(Weapon::Type::Melee, "Loonfungus Sickle", 1, 1, 4, 4, 0, 1),
    m_gnashers(Weapon::Type::Melee, "Gnashers", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, SNEAKY_SNUFFERS};
    m_weapons = {&m_sickle, &m_gnashers};
}

bool SneakySnufflers::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_sickle);
        model->addMeleeWeapon(&m_gnashers);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

Unit *SneakySnufflers::Create(const ParameterList &parameters)
{
    auto unit = new SneakySnufflers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SneakySnufflers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sneaky Snufflers", factoryMethod);
    }
}

} // namespace GloomspiteGitz
