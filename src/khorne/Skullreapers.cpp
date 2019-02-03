/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/Skullreapers.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Skullreapers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Skullreapers::MIN_UNIT_SIZE}, Skullreapers::MIN_UNIT_SIZE,
            Skullreapers::MAX_UNIT_SIZE, Skullreapers::MIN_UNIT_SIZE
        },
        {
            ParamType::Integer, "weapons", {.m_intValue = (int) Skullreapers::WeaponOptions::GoreSlickBlades},
            (int) Skullreapers::WeaponOptions::GoreSlickBlades, (int) Skullreapers::WeaponOptions::Daemonblades, 1
        },
        {ParamType::Integer, "numSpinecleavers", {.m_intValue = 1}, 0, Skullreapers::MAX_UNIT_SIZE / 5, 1},
        {ParamType::Integer, "numSoultearers", {.m_intValue = 0}, 0, Skullreapers::MAX_UNIT_SIZE / 5, 1},
        {ParamType::Boolean, "iconBearer", {.m_boolValue=true}, false, false, false},
    }
};

Weapon Skullreapers::s_goreslickBlades(Weapon::Type::Melee, "Gore-slick Blades", 1, 3, 3, 3, 0, 1);
Weapon Skullreapers::s_daemonblades(Weapon::Type::Melee, "Daemonblades", 1, 3, 4, 3, 0, 1);
Weapon Skullreapers::s_spinecleaver(Weapon::Type::Melee, "Spinecleaver", 1, 2, 3, 3, -1, 2);
Weapon Skullreapers::s_soultearer(Weapon::Type::Melee, "Soultearer", 1, 2, 4, 3, -1, 2);
Weapon Skullreapers::s_viciousMutation(Weapon::Type::Melee, "Vicious Mutation", 1, 1, 3, 4, -1, RAND_D3);

bool Skullreapers::s_registered = false;

Skullreapers::Skullreapers() :
    Unit("Skullreapers", 5, WOUNDS, 7, 4, false)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, SKULLREAPERS};
}

bool Skullreapers::configure(int numModels, Skullreapers::WeaponOptions weapons,
                             int numSplinecleavers, int numSoultearers, bool iconBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    if ((numSplinecleavers + numSoultearers) > numModels / MIN_UNIT_SIZE)
    {
        return false;
    }

    m_weapons = weapons;
    m_iconBearer = iconBearer;
    if (m_iconBearer)
    {
        m_bravery += 1;
    }

    Model skullseeker(BASESIZE, WOUNDS);
    skullseeker.addMeleeWeapon(&s_viciousMutation);
    if (weapons == GoreSlickBlades)
    {
        skullseeker.addMeleeWeapon(&s_goreslickBlades);
    }
    else
    {
        skullseeker.addMeleeWeapon(&s_daemonblades);
    }
    addModel(skullseeker);

    for (auto i = 0; i < numSplinecleavers; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_spinecleaver);
        addModel(model);
    }
    for (auto i = 0; i < numSoultearers; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_soultearer);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == GoreSlickBlades)
        {
            model.addMeleeWeapon(&s_goreslickBlades);
        }
        else
        {
            model.addMeleeWeapon(&s_daemonblades);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Skullreapers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_goreslickBlades);
    visitor(&s_daemonblades);
    visitor(&s_spinecleaver);
    visitor(&s_soultearer);
    visitor(&s_viciousMutation);
}

Unit *Skullreapers::Create(const ParameterList &parameters)
{
    auto unit = new Skullreapers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    auto weapons = (Skullreapers::WeaponOptions) GetIntParam("weapons", parameters, Skullreapers::GoreSlickBlades);
    int numSplinecleavers = GetIntParam("numSpinecleavers", parameters, 1);
    int numSoultearers = GetIntParam("numSoultearers", parameters, 0);
    bool iconBearer = GetBoolParam("iconBearer", parameters, true);

    bool ok = unit->configure(numModels, weapons, numSplinecleavers, numSoultearers, iconBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Skullreapers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Skullreapers", factoryMethod);
    }
}

Rerolls Skullreapers::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Trial of skulls.
    if (m_numEnemyModelsSlain > remainingModels())
    {
        return RerollFailed;
    }
    // Frenzied Attacks
    if (weapon->name() == s_daemonblades.name() || weapon->name() == s_goreslickBlades.name())
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, target);
}

int Skullreapers::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits, const WoundingHits& wounds)
{
    // Daemonforged Weapons
    if (weapon->name() == s_daemonblades.name() || weapon->name() == s_soultearer.name())
    {
        int mortalWounds = hits.rolls.numUnmodified6s();

        int selfWounds = hits.rolls.numUnmodified1s();
        if (selfWounds)
        {
            applyDamage({0, selfWounds});
        }

        return mortalWounds;
    }

    return Unit::generateMortalWounds(weapon, unit, hits, wounds);
}

void Skullreapers::onSlain()
{
    // Murderous to the Last
    if (m_modelsSlain && m_meleeTarget)
    {
        Dice dice;
        Dice::RollResult rolls;
        dice.rollD6(m_modelsSlain, rolls);
        int mortalWounds = rolls.distribution[4] + rolls.distribution[5];
        for (int i = 0; i < rolls.numUnmodified6s(); i++)
        {
            mortalWounds += dice.rollD3();
        }
        m_meleeTarget->applyDamage({0, mortalWounds});
    }
    Unit::onSlain();
}

void Skullreapers::onStartCombat(PlayerId player)
{
    Unit::onStartCombat(player);

    if (m_meleeTarget)
    {
        m_initialTargetModels = m_meleeTarget->remainingModels();
    }
}

void Skullreapers::onEndCombat(PlayerId player)
{
    Unit::onEndCombat(player);

    // Trial of Skulls
    if (m_meleeTarget)
    {
        m_numEnemyModelsSlain += std::max(0, m_initialTargetModels - m_meleeTarget->remainingModels());
    }
}

} //namespace Khorne