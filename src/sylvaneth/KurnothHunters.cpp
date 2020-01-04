/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/KurnothHunters.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    KurnothHunters::Create,
    KurnothHunters::ValueToString,
    KurnothHunters::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", KurnothHunters::MIN_UNIT_SIZE, KurnothHunters::MIN_UNIT_SIZE,
            KurnothHunters::MAX_UNIT_SIZE, KurnothHunters::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", KurnothHunters::Greatswords, KurnothHunters::Greatswords,
            KurnothHunters::Greatbows, 1
        },
        {ParamType::Enum, "Glade", SylvanethBase::None, SylvanethBase::None, SylvanethBase::Harvestboon, 1},
    },
    ORDER,
    SYLVANETH
};

bool KurnothHunters::s_registered = false;

KurnothHunters::KurnothHunters() :
    SylvanethBase("Kurnoth Hunters", 5, WOUNDS, 7, 4, false),
    m_greatbow(Weapon::Type::Missile, "Kurnoth Greatbow", 30, 2, 4, 3, -1, RAND_D3),
    m_greatbowHuntmaster(Weapon::Type::Missile, "Kurnoth Greatbow", 30, 2, 3, 3, -1, RAND_D3),
    m_greatsword(Weapon::Type::Melee, "Kurnoth Greatsword", 1, 4, 3, 3, -1, 2),
    m_greatswordHuntmaster(Weapon::Type::Melee, "Kurnoth Greatsword", 1, 4, 2, 3, -1, 2),
    m_scythe(Weapon::Type::Melee, "Kurnoth Scythe", 2, 3, 3, 3, -2, RAND_D3),
    m_scytheHuntmaster(Weapon::Type::Melee, "Kurnoth Scythe", 2, 3, 2, 3, -2, RAND_D3),
    m_viciousClaws(Weapon::Type::Melee, "Quiverling's Vicious Claws", 1, 3, 4, 4, 0, 1)
{
    m_keywords = {ORDER, SYLVANETH, FREE_SPIRITS, KURNOTH_HUNTERS};
}

bool KurnothHunters::configure(int numModels, WeaponOption weapons)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_weaponOption = weapons;

    auto huntmaster = new Model(BASESIZE, WOUNDS);
    if (m_weaponOption == Greatbows)
    {
        huntmaster->addMissileWeapon(&m_greatbowHuntmaster);
        huntmaster->addMeleeWeapon(&m_viciousClaws);
    }
    else if (m_weaponOption == Greatswords)
    {
        huntmaster->addMeleeWeapon(&m_greatswordHuntmaster);
    }
    else if (m_weaponOption == Scythes)
    {
        huntmaster->addMeleeWeapon(&m_scytheHuntmaster);
    }
    addModel(huntmaster);

    for (int i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (m_weaponOption == Greatbows)
        {
            model->addMissileWeapon(&m_greatbow);
            model->addMeleeWeapon(&m_viciousClaws);
        }
        else if (m_weaponOption == Greatswords)
        {
            model->addMeleeWeapon(&m_greatsword);
        }
        else if (m_weaponOption == Scythes)
        {
            model->addMeleeWeapon(&m_scythe);
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

Unit *KurnothHunters::Create(const ParameterList &parameters)
{
    auto unit = new KurnothHunters();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, KurnothHunters::Greatswords);

    auto glade = (Glade)GetEnumParam("Glade", parameters, SylvanethBase::None);
    unit->setGlade(glade);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KurnothHunters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Kurnoth Hunters", factoryMethod);
    }
}

std::string KurnothHunters::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == Greatswords)
        {
            return "Greatswords";
        }
        else if (parameter.m_intValue == Scythes)
        {
            return "Scythes";
        }
        else if (parameter.m_intValue == Greatbows)
        {
            return "Greatbows";
        }
    }
    return ParameterValueToString(parameter);
}

int KurnothHunters::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Greatswords")
    {
        return Greatswords;
    }
    else if (enumString == "Scythes")
    {
        return Scythes;
    }
    else if (enumString == "Greatbows")
    {
        return Greatbows;
    }
    return 0;
}

void KurnothHunters::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_greatbow);
    visitor(m_greatbowHuntmaster);
    visitor(m_greatsword);
    visitor(m_greatswordHuntmaster);
    visitor(m_scythe);
    visitor(m_scytheHuntmaster);
    visitor(m_viciousClaws);
}

Wounds KurnothHunters::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Sundering Strike
    if ((weapon->name() == m_greatsword.name()) && (woundRoll == 6))
    {
        return {weapon->damage(), 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Wounds KurnothHunters::onEndCombat(PlayerId player)
{
    auto wounds = Unit::onEndCombat(player);

    // Trample Underfoot
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 1.0f);
    if (!units.empty())
    {
        auto unit = units.front();
        Dice dice;
        Dice::RollResult result;
        dice.rollD6(unit->remainingModels(), result);
        Wounds trampleWounds = {0, result.rollsGE(4)};
        unit->applyDamage(trampleWounds);
        wounds += trampleWounds;
    }
    return wounds;
}

} // namespace Sylvaneth
