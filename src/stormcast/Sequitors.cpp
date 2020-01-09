/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/Sequitors.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Sequitors::Create,
    Sequitors::ValueToString,
    Sequitors::EnumStringToInt,
    {
        {ParamType::Integer, "Models", Sequitors::MIN_UNIT_SIZE, Sequitors::MIN_UNIT_SIZE, Sequitors::MAX_UNIT_SIZE, Sequitors::MIN_UNIT_SIZE},
        {
            ParamType::Enum, "Weapons", Sequitors::StormsmiteMaul, Sequitors::StormsmiteMaul,
            Sequitors::TempestBlade, 1
        },
        {ParamType::Integer, "Greatmaces", 2, 0, Sequitors::MAX_UNIT_SIZE / 5 * 2, 1},
        {ParamType::Boolean, "Prime Greatmace", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Redemption Cache", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Stormhost", StormcastEternal::None, StormcastEternal::None, StormcastEternal::AstralTemplars, 1},
    },
    ORDER,
    STORMCAST_ETERNAL
};

bool Sequitors::s_registered = false;

Sequitors::Sequitors() :
    StormcastEternal("Sequitors", 5, WOUNDS, 7, 4, false),
    m_stormsmiteMaul(Weapon::Type::Melee, "Stormsmite Maul", 1, 2, 3, 3, 0, 1),
    m_stormsmiteMaulPrime(Weapon::Type::Melee, "Stormsmite Maul", 1, 3, 3, 3, 0, 1),
    m_tempestBlade(Weapon::Type::Melee, "Tempest Blade", 1, 3, 3, 4, 0, 1),
    m_tempestBladePrime(Weapon::Type::Melee, "Tempest Blade", 1, 4, 3, 4, 0, 1),
    m_stormsmiteGreatmace(Weapon::Type::Melee, "Stormsmite Greatmace", 1, 2, 3, 3, -1, 2),
    m_stormsmiteGreatmacePrime(Weapon::Type::Melee, "Stormsmite Greatmace", 1, 3, 3, 3, -1, 2),
    m_redemptionCache(Weapon::Type::Missile, "Redemption Cache", 6, 1, 4, 0, 0, 1)
{
    m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, REDEEMER, SEQUITORS};
    m_weapons = {&m_stormsmiteMaul,
                 &m_stormsmiteMaulPrime,
                 &m_tempestBlade,
                 &m_tempestBladePrime,
                 &m_stormsmiteGreatmace,
                 &m_stormsmiteGreatmacePrime,
                 &m_redemptionCache};
}

bool Sequitors::configure(int numModels, WeaponOption weapons, int numGreatmaces, bool primeGreatmace, bool redemptionCache)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxGrandmaces = (numModels / 5) * 2;
    if (numGreatmaces > maxGrandmaces)
    {
        // Invalid weapon configuration.
        return false;
    }
    if (primeGreatmace && redemptionCache)
    {
        // Invalid weapon configuration.
        return false;
    }

    m_weaponOption = weapons;
    m_haveRedemptionCache = redemptionCache;

    // Add the Prime
    auto primeModel = new Model(BASESIZE, WOUNDS);
    if (primeGreatmace)
    {
        primeModel->addMeleeWeapon(&m_stormsmiteGreatmacePrime);
    }
    else if (m_weaponOption == StormsmiteMaul)
    {
        primeModel->addMeleeWeapon(&m_stormsmiteMaulPrime);
    }
    else if (m_weaponOption == TempestBlade)
    {
        primeModel->addMeleeWeapon(&m_tempestBladePrime);
    }
    if (m_haveRedemptionCache)
    {
        primeModel->addMissileWeapon(&m_redemptionCache);
    }
    addModel(primeModel);

    for (auto i = 0; i < numGreatmaces; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_stormsmiteGreatmace);
        addModel(model);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (m_weaponOption == StormsmiteMaul)
        {
            model->addMeleeWeapon(&m_stormsmiteMaul);
        }
        else if (m_weaponOption == TempestBlade)
        {
            model->addMeleeWeapon(&m_tempestBlade);
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

Rerolls Sequitors::toSaveRerolls(const Weapon *weapon) const
{
    const bool isMissile = weapon->isMissile();

    // Soulshields Shields
    for (const auto &m : m_models)
    {
        // check if remaining models have a shield
        for (auto ip = m->meleeWeaponBegin(); ip != m->meleeWeaponEnd(); ++ip)
        {
            if ((*ip)->name() == m_stormsmiteMaul.name() || (*ip)->name() == m_tempestBlade.name())
            {
                if (m_aethericChannellingWeapons || isMissile)
                {
                    return RerollOnes; // weapons empowered
                }
                else
                {
                    return RerollFailed;
                } // shields empowered
            }
        }
    }

    return StormcastEternal::toSaveRerolls(weapon);
}

Rerolls Sequitors::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Aetheric Channeling
    if (m_aethericChannellingWeapons)
    {
        return RerollFailed;
    }
    return StormcastEternal::toHitRerolls(weapon, unit);
}

int Sequitors::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const
{
    // Greatmace Blast
    if (unmodifiedHitRoll == 6)
    {
        if ((weapon->name() == m_stormsmiteGreatmace.name()) &&
            (unit->hasKeyword(DAEMON) || unit->hasKeyword(NIGHTHAUNT)))
        {
            Dice dice;
            // each 6 inflicts d3 hits instead of 1
            return dice.rollD3();
        }
    }
    return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
}

Unit *Sequitors::Create(const ParameterList &parameters)
{
    auto unit = new Sequitors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, StormsmiteMaul);
    int numGreatmaces = GetIntParam("Greatmaces", parameters, 0);
    bool primeGreatmace = GetBoolParam("Prime Greatmace", parameters, false);
    bool redemptionCache = GetBoolParam("Redemption Cache", parameters, false);

    auto stormhost = (Stormhost)GetEnumParam("Stormhost", parameters, StormcastEternal::None);
    unit->setStormhost(stormhost);

    bool ok = unit->configure(numModels, weapons, numGreatmaces, primeGreatmace, redemptionCache);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Sequitors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Sequitors", factoryMethod);
    }
}

std::string Sequitors::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == StormsmiteMaul)
        {
            return "Stormsmite Maul";
        }
        else if (parameter.intValue == TempestBlade)
        {
            return "Tempest Blade";
        }
    }
    return StormcastEternal::ValueToString(parameter);
}

int Sequitors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Stormsmite Maul")
    {
        return StormsmiteMaul;
    }
    else if (enumString == "Tempest Blade")
    {
        return TempestBlade;
    }
    return StormcastEternal::EnumStringToInt(enumString);
}

void Sequitors::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    if ((player == m_owningPlayer) && m_haveRedemptionCache)
    {
        Dice dice;
        // Redemption Cache
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 6.0f);
        for (auto ip : units)
        {
            if (ip->hasKeyword(CHAOS) || ip->hasKeyword(DEATH))
            {
                int roll = dice.rollD6();
                if (roll >= 4)
                {
                    ip->applyDamage({0, 1});
                    break;
                }
            }
        }
    }
}

} // namespace StormcastEternals
