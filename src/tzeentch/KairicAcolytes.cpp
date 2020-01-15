/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/KairicAcolytes.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Tzeentch
{
static FactoryMethod factoryMethod = {
    KairicAcolytes::Create,
    KairicAcolytes::ValueToString,
    KairicAcolytes::EnumStringToInt,
    KairicAcolytes::ComputePoints,
    {
        {
            ParamType::Integer, "Models", KairicAcolytes::MIN_UNIT_SIZE, KairicAcolytes::MIN_UNIT_SIZE,
            KairicAcolytes::MAX_UNIT_SIZE, KairicAcolytes::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", KairicAcolytes::CursedBlade, KairicAcolytes::CursedBlade,
            KairicAcolytes::CursedBladeAndShield, 1
        },
        {ParamType::Integer, "Cursed Glaives", 0, 0, KairicAcolytes::MAX_UNIT_SIZE/KairicAcolytes::MIN_UNIT_SIZE*3, 1},
        {ParamType::Integer, "Scrolls Of Dark Arts", 0, 0, KairicAcolytes::MAX_UNIT_SIZE/KairicAcolytes::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Vulcharcs", 0, 0, KairicAcolytes::MAX_UNIT_SIZE/KairicAcolytes::MIN_UNIT_SIZE, 1},
    },
    CHAOS,
    { TZEENTCH }
};

bool KairicAcolytes::s_registered = false;

KairicAcolytes::KairicAcolytes() :
    Unit("Kairic Acolytes", 6, WOUNDS, 5, 5, false),
    m_sorcerousBolt(Weapon::Type::Missile, "Sorcerous Bolt", 18, 1, 4, 3, 0, 1),
    m_cursedBlade(Weapon::Type::Melee, "Cursed Blade", 1, 1, 4, 3, 0, 1),
    m_cursedGlaive(Weapon::Type::Melee, "Cursed Glaive", 1, 1, 4, 3, -1, 2),
    m_cursedBladeAdept(Weapon::Type::Melee, "Cursed Blade", 1, 2, 4, 3, 0, 1),
    m_cursedGlaiveAdept(Weapon::Type::Melee, "Cursed Glaive", 1, 2, 4, 3, -1, 2)
{
    m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, KAIRIC_ACOLYTES};
    m_weapons = {&m_sorcerousBolt, &m_cursedBlade, &m_cursedGlaive, &m_cursedBladeAdept, &m_cursedGlaiveAdept};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool KairicAcolytes::configure(int numModels, WeaponOptions weapons, int numCursedGlaives, int numScrollsOfDarkArts, int numVulcharcs)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    const int maxGlaives = (numModels / 10) * 3;
    if (numCursedGlaives > maxGlaives)
    {
        // Invalid weapon configuration.
        return false;
    }
    const int maxScrolls = numModels / 10;
    const int maxVulcharcs = numModels / 10;
    if (numScrollsOfDarkArts > maxScrolls || numVulcharcs > maxVulcharcs)
    {
        return false;
    }

    m_weaponOption = weapons;
    m_numCursedGlaives = numCursedGlaives;
    m_numScrollsOfDarkArts = numScrollsOfDarkArts;
    m_numVulcharcs = numVulcharcs;

    auto adept = new Model(BASESIZE, WOUNDS);
    adept->addMissileWeapon(&m_sorcerousBolt);
    adept->addMeleeWeapon(&m_cursedBladeAdept);
    addModel(adept);

    for (auto i = 0; i < m_numCursedGlaives; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_sorcerousBolt);
        model->addMeleeWeapon(&m_cursedGlaive);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_sorcerousBolt);
        model->addMeleeWeapon(&m_cursedBlade);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Wounds KairicAcolytes::applyWoundSave(const Wounds &wounds)
{
    // Arcanite Shield
    if (m_weaponOption == CursedBladeAndShield)
    {
    }
    return Unit::applyWoundSave(wounds);
}

void KairicAcolytes::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Kairic Acolytes", factoryMethod);
    }
}

Unit *KairicAcolytes::Create(const ParameterList &parameters)
{
    auto unit = new KairicAcolytes();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOptions)GetEnumParam("Weapons", parameters, CursedBlade);
    int numCursedGlaives = GetIntParam("Cursed Glaives", parameters, 0);
    int numScrollsOfDarkArts = GetIntParam("Scrolls Of Dark Arts", parameters, 0);
    int numVulcharcs = GetIntParam("Vulcharcs", parameters, 0);

    bool ok = unit->configure(numModels, weapons, numCursedGlaives, numScrollsOfDarkArts, numVulcharcs);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string KairicAcolytes::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == CursedBlade)
        {
            return "Cursed Blade";
        }
        else if (parameter.intValue == PairedCursedBlades)
        {
            return "Paired Cursed Blades";
        }
        else if (parameter.intValue == CursedBladeAndShield)
        {
            return "Cursed Blade And Shield";
        }
    }

    return ParameterValueToString(parameter);
}

int KairicAcolytes::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Cursed Blade")
    {
        return CursedBlade;
    }
    else if (enumString == "Paired Cursed Blades")
    {
        return PairedCursedBlades;
    }
    else if (enumString == "Cursed Blade And Shield")
    {
        return CursedBladeAndShield;
    }
    return 0;
}

int KairicAcolytes::castingModifier() const
{
    int modifier = Unit::castingModifier();

    // Scroll of Dark Arts
    if (m_numScrollsOfDarkArts) modifier++;

    return modifier;
}

Rerolls KairicAcolytes::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Paired Cursed Blades
    if ((m_weaponOption == PairedCursedBlades) && (weapon->name() == m_cursedBlade.name()))
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

int KairicAcolytes::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Tzeentch