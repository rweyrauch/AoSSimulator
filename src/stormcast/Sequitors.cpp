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

namespace StormcastEternals
{
static FactoryMethod factoryMethod = {
    Sequitors::Create,
    Sequitors::ValueToString,
    Sequitors::EnumStringToInt,
    {
        {ParamType::Integer, "numModels", {.m_intValue = 5}, Sequitors::MIN_UNIT_SIZE, Sequitors::MAX_UNIT_SIZE, Sequitors::MIN_UNIT_SIZE},
        {ParamType::Integer, "weapons", {.m_intValue = Sequitors::StormsmiteMaul}, Sequitors::StormsmiteMaul,
         Sequitors::TempestBlade, 1},
        {ParamType::Integer, "numGreatmaces", {.m_intValue = 2}, 0, Sequitors::MAX_UNIT_SIZE / 5 * 2},
        {ParamType::Boolean, "primeGreatmace", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "redemptionCache", {.m_boolValue = false}, false, false}
    }
};

bool Sequitors::s_registered = false;

Weapon Sequitors::s_stormsmiteMaul(Weapon::Type::Melee, "Stormsmite Maul", 1, 2, 3, 3, 0, 1);
Weapon Sequitors::s_tempestBlade(Weapon::Type::Melee, "Tempest Blade", 1, 3, 3, 4, 0, 1);
Weapon Sequitors::s_stormsmiteGreatmace(Weapon::Type::Melee, "Stormsmite Greatmace", 1, 2, 3, 3, -1, 2);

Weapon Sequitors::s_stormsmiteMaulPrime(Weapon::Type::Melee, "Stormsmite Maul (Prime)", 1, 3, 3, 3, 0, 1);
Weapon Sequitors::s_tempestBladePrime(Weapon::Type::Melee, "Tempest Blade (Prime)", 1, 4, 3, 4, 0, 1);
Weapon Sequitors::s_stormsmiteGreatmacePrime(Weapon::Type::Melee, "Stormsmite Greatmace (Prime)", 1, 3, 3, 3, -1, 2);

Weapon Sequitors::s_redemptionCache(Weapon::Type::Missile, "Redemption Cache", 6, 1, 4, 0, 0, 1);

Sequitors::Sequitors() :
    StormcastEternal("Sequitors", 5, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, REDEEMER, SEQUITORS };
}

bool Sequitors::configure(int numModels, WeaponOption weapons, int numGreatmaces, bool primeGreatmace, bool redemptionCache)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }
    const int maxGrandmaces = (numModels / 5)*2;
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
    m_redemptionCache = redemptionCache;

    // Add the Prime
    Model primeModel(BASESIZE, WOUNDS);
    if (primeGreatmace)
    {
        primeModel.addMeleeWeapon(&s_stormsmiteGreatmacePrime);
    }
    else if (m_weaponOption == StormsmiteMaul)
    {
        primeModel.addMeleeWeapon(&s_stormsmiteMaulPrime);
    }
    else if (m_weaponOption == TempestBlade)
    {
        primeModel.addMeleeWeapon(&s_tempestBladePrime);
    }
    if (m_redemptionCache)
    {
        primeModel.addMissileWeapon(&s_redemptionCache);
    }
    addModel(primeModel);

    for (auto i = 0; i < numGreatmaces; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_stormsmiteGreatmace);
        addModel(model);
    }

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == StormsmiteMaul)
            model.addMeleeWeapon(&s_stormsmiteMaul);
        else if (m_weaponOption == TempestBlade)
            model.addMeleeWeapon(&s_tempestBlade);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_stormsmiteMaul.name() << ": " << s_stormsmiteMaul.strength() << std::endl;
        std::cout << "\t" << s_stormsmiteMaulPrime.name() << ": " << s_stormsmiteMaulPrime.strength() << std::endl;
        std::cout << "\t" << s_tempestBlade.name() << ": " << s_tempestBlade.strength() << std::endl;
        std::cout << "\t" << s_tempestBladePrime.name() << ": " << s_tempestBladePrime.strength() << std::endl;
        std::cout << "\t" << s_stormsmiteGreatmace.name() << ": " << s_stormsmiteGreatmace.strength() << std::endl;
        std::cout << "\t" << s_stormsmiteGreatmacePrime.name() << ": " << s_stormsmiteGreatmacePrime.strength() << std::endl;
    }
    return true;
}

Rerolls Sequitors::toSaveRerolls(const Weapon* weapon) const
{
    const bool isMissile = weapon->isMissile();

    // Soulshields Shields
    for (const auto &m : m_models)
    {
        // check if remaining models have a shield
        for (auto ip = m.meleeWeaponBegin(); ip != m.meleeWeaponEnd(); ++ip)
        {
            if ((*ip)->name() == s_stormsmiteMaul.name() || (*ip)->name() == s_tempestBlade.name())
            {
                if (m_aethericChannellingWeapons || isMissile)
                    return RerollOnes; // weapons empowered
                else
                    return RerollFailed; // shields empowered
            }
        }
    }

    return NoRerolls;
}

Rerolls Sequitors::toHitRerolls(const Weapon* weapon, const Unit *unit) const
{
    if (m_aethericChannellingWeapons)
        return RerollFailed;
    return StormcastEternal::toHitRerolls(weapon, unit);
}

Hits Sequitors::applyHitModifiers(const Weapon *weapon, const Unit *unit, const Hits &hits) const
{
    Hits modifiedHits = hits;
    if ((hits.rolls.numUnmodified6s() > 0) && (weapon->name() == s_stormsmiteGreatmace.name()) &&
        (unit->hasKeyword(DAEMON) || unit->hasKeyword(NIGHTHAUNT)))
    {
        // Greatmace Blast
        Dice dice;
        // each 6 inflicts d3 hits instead of 1
        modifiedHits.numHits -= hits.rolls.numUnmodified6s();
        for (auto i = 0; i < hits.rolls.numUnmodified6s(); i++)
        {
            modifiedHits.numHits += dice.rollD3();
        }
    }

    // modifiers accumulate
    return Unit::applyHitModifiers(weapon, unit, modifiedHits);
}

Unit *Sequitors::Create(const ParameterList &parameters)
{
    auto unit = new Sequitors();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption)GetIntParam("weapons", parameters, StormsmiteMaul);
    int numGreatmaces = GetIntParam("numGreatmaces", parameters, 0);
    bool primeGreatmace = GetBoolParam("primeGreatmace", parameters, false);
    bool redemptionCache = GetBoolParam("redemptionCache", parameters, false);

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
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == StormsmiteMaul)
            return "StormsmiteMaul";
        else if (parameter.m_intValue == TempestBlade)
            return "TempestBlade";
    }
    return ParameterValueToString(parameter);
}

int Sequitors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "StormsmiteMaul")
        return StormsmiteMaul;
    else if (enumString == "TempestBlade")
        return TempestBlade;
    return 0;
}

} // namespace StormcastEternals
