/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/Khorgoraths.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Khorgoraths::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", Khorgoraths::MIN_UNIT_SIZE,
            Khorgoraths::MIN_UNIT_SIZE,
            Khorgoraths::MAX_UNIT_SIZE, Khorgoraths::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    KHORNE
};

bool Khorgoraths::s_registered = false;


Khorgoraths::Khorgoraths() :
    Unit("Khorgoraths", 6, WOUNDS, 6, 4, false),
    m_boneTentacles(Weapon::Type::Missile, "Bone Tentacles", 6, 3, 3, 4, 0, 1),
    m_clawAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 5, 3, 3, -1, 2)
{
    m_keywords = {CHAOS, KHORNE, MONSTER, BLOODBOUND, KHORGORATHS};
}

bool Khorgoraths::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_boneTentacles);
        model.addMeleeWeapon(&m_clawAndFangs);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Khorgoraths::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_boneTentacles);
    visitor(&m_clawAndFangs);
}

Unit *Khorgoraths::Create(const ParameterList &parameters)
{
    auto unit = new Khorgoraths();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Khorgoraths::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Khorgoraths", factoryMethod);
    }
}

void Khorgoraths::onStartCombat(PlayerId player)
{
    m_modelsSlainAtStartOfCombat = m_currentRecord.m_enemyModelsSlain;

    Unit::onStartCombat(player);
}

void Khorgoraths::onEndCombat(PlayerId player)
{
    // Taker of Heads
    if (m_currentRecord.m_enemyModelsSlain > m_modelsSlainAtStartOfCombat)
    {
        SimLog(Verbosity::Narrative, "%s slayed %d models this combat phase and heals 1 wound.\n",
            name().c_str(), (m_currentRecord.m_enemyModelsSlain - m_modelsSlainAtStartOfCombat));
        heal(1);
    }
    Unit::onEndCombat(player);
}

} //namespace Khorne

