/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/HearthguardBerzerkers.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    HearthguardBerzerkers::Create,
    HearthguardBerzerkers::ValueToString,
    HearthguardBerzerkers::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", HearthguardBerzerkers::MIN_UNIT_SIZE, HearthguardBerzerkers::MIN_UNIT_SIZE,
            HearthguardBerzerkers::MAX_UNIT_SIZE, HearthguardBerzerkers::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", HearthguardBerzerkers::BerzerkerBroadaxe, HearthguardBerzerkers::BerzerkerBroadaxe, HearthguardBerzerkers::FlamestrikePoleaxe, 1},
        {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
    },
    ORDER,
    FYRESLAYERS
};

bool HearthguardBerzerkers::s_registered = false;

HearthguardBerzerkers::HearthguardBerzerkers() :
    Fyreslayer("Hearthguard Berzerkers", 4, WOUNDS, 8, 5, false),
    m_broadaxe(Weapon::Type::Melee, "Berzerker Broadaxe", 2, 2, 3, 3, -1, 2),
    m_broadaxeKarl(Weapon::Type::Melee, "Berzerker Broadaxe", 2, 3, 3, 3, -1, 2),
    m_poleaxe(Weapon::Type::Melee, "Flamestrike Poleaxe", 2, 2, 3, 3, 0, 1),
    m_poleaxeKarl(Weapon::Type::Melee, "Flamestrike Poleaxe", 2, 3, 3, 3, 0, 1),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HEARTHGUARD_BERZERKERS};
}

bool HearthguardBerzerkers::configure(int numModels, WeaponOption weapons)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model karl(BASESIZE, WOUNDS);
    karl.addMissileWeapon(&m_throwingAxe);
    if (weapons == BerzerkerBroadaxe)
    {
        karl.addMeleeWeapon(&m_broadaxeKarl);
    }
    else if (weapons == FlamestrikePoleaxe)
    {
        karl.addMeleeWeapon(&m_poleaxeKarl);
    }
    addModel(karl);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_throwingAxe);
        if (weapons == BerzerkerBroadaxe)
        {
            model.addMeleeWeapon(&m_broadaxe);
        }
        else if (weapons == FlamestrikePoleaxe)
        {
            model.addMeleeWeapon(&m_poleaxe);
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

void HearthguardBerzerkers::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_broadaxe);
    visitor(m_broadaxeKarl);
    visitor(m_poleaxe);
    visitor(m_poleaxeKarl);
    visitor(m_throwingAxe);
}

Unit *HearthguardBerzerkers::Create(const ParameterList &parameters)
{
    auto unit = new HearthguardBerzerkers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, BerzerkerBroadaxe);

    auto lodge = (Lodge) GetEnumParam("Lodge", parameters, Fyreslayer::None);
    unit->setLodge(lodge);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void HearthguardBerzerkers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Hearthguard Berzerkers", factoryMethod);
    }
}

std::string HearthguardBerzerkers::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == BerzerkerBroadaxe) { return "Berzerker Broadaxe"; }
        else if (parameter.m_intValue == FlamestrikePoleaxe) { return "Flamestrike Poleaxe"; }
    }
    return Fyreslayer::ValueToString(parameter);
}

int HearthguardBerzerkers::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Berzerker Broadaxe") { return BerzerkerBroadaxe; }
    else if (enumString == "Flamestrike Poleaxe") { return FlamestrikePoleaxe; }
    return Fyreslayer::EnumStringToInt(enumString);
}

Wounds HearthguardBerzerkers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Smouldering Boulders
    if ((hitRoll == 6) && (weapon->name() == m_poleaxe.name()))
    {
        return {weapon->damage(), 2};
    }
    return Fyreslayer::weaponDamage(weapon, target, hitRoll, woundRoll);
}

Wounds HearthguardBerzerkers::applyWoundSave(const Wounds &wounds)
{
    // Duty Unto Death
    Dice dice;
    Dice::RollResult woundSaves, mortalSaves;
    dice.rollD6(wounds.normal, woundSaves);
    dice.rollD6(wounds.mortal, mortalSaves);

    int saveValue = 6;

    auto unit = Board::Instance()->getUnitWithKeyword(this, m_owningPlayer, HERO, 10.0f); // TODO: must have both HERO and FYRESLAYER keywords
    if (unit)
    {
        saveValue -= 2;
    }

    Wounds totalWounds = wounds;
    totalWounds.normal -= woundSaves.rollsGE(saveValue);
    totalWounds.normal = std::max(totalWounds.normal, 0);
    totalWounds.mortal -= mortalSaves.rollsGE(saveValue);
    totalWounds.mortal = std::max(totalWounds.mortal, 0);

    return totalWounds;
}

} //namespace Fyreslayers