/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/Orruks.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Greenskinz
{
static FactoryMethod factoryMethod = {
    Orruks::Create,
    Orruks::ValueToString,
    Orruks::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = Orruks::MIN_UNIT_SIZE},
            Orruks::MIN_UNIT_SIZE, Orruks::MAX_UNIT_SIZE, Orruks::MIN_UNIT_SIZE
        },
        {ParamType::Enum, "Weapons", {.m_intValue = Orruks::ChoppaAndShield}, Orruks::ChoppaAndShield, Orruks::OrrukBowAndCutta, 1},
        {ParamType::Boolean, "Waaagh! Drummer", {.m_boolValue = false}, false, false, false},
        {ParamType::Enum, "Standard Bearer", {.m_intValue = Orruks::OrrukBanner}, Orruks::None, Orruks::SkullIcon, 1},
    },
    DESTRUCTION,
    GREENSKINZ
};

bool Orruks::s_registered = false;

Orruks::Orruks() :
    Unit("Orruks", 5, WOUNDS, 5, 5, false),
    m_orrukBows(Weapon::Type::Missile, "Orruk Bows", 18, 1, 5, 4, 0, 1),
    m_choppa(Weapon::Type::Melee, "Choppa", 1, 1, 4, 4, -1, 1),
    m_pigstikkaSpear(Weapon::Type::Melee, "Pigstikka Spear", 2, 1, 4, 4, 0, 1),
    m_cutta(Weapon::Type::Melee, "Cutta", 1, 1, 4, 5, 0, 1),
    m_orrukBowBoss(Weapon::Type::Missile, "Orruk Bow (Boss)", 18, 2, 5, 4, 0, 1),
    m_choppaBoss(Weapon::Type::Melee, "Choppa (Boss)", 1, 2, 4, 4, -1, 1),
    m_pigstikkaSpearBoss(Weapon::Type::Melee, "Pigstikka Spear (Boss)", 2, 2, 4, 4, 0, 1),
    m_cuttaBoss(Weapon::Type::Melee, "Cutta (Boss)", 1, 2, 4, 5, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, ORRUKS};
}

bool Orruks::configure(int numModels, WeaponOption weapons, bool drummer, StandardBearer standardBearer)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_waaaghDrummer = drummer;
    m_standardBearer = standardBearer;
    m_weapons = weapons;

    // Add the boss
    Model boss(BASESIZE, WOUNDS);
    switch (weapons)
    {
        case ChoppaAndShield:
            boss.addMeleeWeapon(&m_choppaBoss);
            break;
        case SpearAndShield:
            boss.addMeleeWeapon(&m_pigstikkaSpearBoss);
            break;
        case PairedChoppas:
            boss.addMeleeWeapon(&m_choppaBoss);
            break;
        case OrrukBowAndCutta:
            boss.addMissileWeapon(&m_orrukBowBoss);
            boss.addMeleeWeapon(&m_choppaBoss);
            break;
    }
    addModel(boss);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        switch (weapons)
        {
            case ChoppaAndShield:
                model.addMeleeWeapon(&m_choppa);
                break;
            case SpearAndShield:
                model.addMeleeWeapon(&m_pigstikkaSpear);
                break;
            case PairedChoppas:
                model.addMeleeWeapon(&m_choppa);
                break;
            case OrrukBowAndCutta:
                model.addMissileWeapon(&m_orrukBows);
                model.addMeleeWeapon(&m_choppa);
                break;
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

void Orruks::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_orrukBows);
    visitor(&m_orrukBowBoss);
    visitor(&m_choppa);
    visitor(&m_choppaBoss);
    visitor(&m_pigstikkaSpear);
    visitor(&m_pigstikkaSpearBoss);
    visitor(&m_cutta);
    visitor(&m_cuttaBoss);
}

Unit *Orruks::Create(const ParameterList &parameters)
{
    auto unit = new Orruks();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, ChoppaAndShield);
    bool drummer = GetBoolParam("Waaagh! Drummer", parameters, false);
    StandardBearer standardBearer = (StandardBearer) GetEnumParam("Standard Bearer", parameters, OrrukBanner);

    bool ok = unit->configure(numModels, weapons, drummer, standardBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Orruks::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Orruks", factoryMethod);
    }
}

std::string Orruks::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == ChoppaAndShield)
        {
            return "Choppa and Shield";
        }
        else if (parameter.m_intValue == SpearAndShield)
        {
            return "Pigstikka Spear And Shield";
        }
        else if (parameter.m_intValue == PairedChoppas)
        {
            return "Paired Choppas";
        }
        else if (parameter.m_intValue == OrrukBowAndCutta)
        {
            return "Orruk Bow and Cutta";
        }
    }
    else if (parameter.m_name == "Standard Bearer")
    {
        if (parameter.m_intValue == OrrukBanner)
        {
            return "Orruk Banner";
        }
        else if (parameter.m_intValue == SkullIcon)
        {
            return "Skull Icon";
        }
        else if (parameter.m_intValue == None)
        {
            return "None";
        }
    }
    return ParameterValueToString(parameter);
}

int Orruks::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Choppa and Shield")
    {
        return ChoppaAndShield;
    }
    else if (enumString == "Pigstikka Spear and Shield")
    {
        return SpearAndShield;
    }
    else if (enumString == "Paired Choppas")
    {
        return PairedChoppas;
    }
    else if (enumString == "Orruk Bow and Cutta")
    {
        return OrrukBowAndCutta;
    }
    else if (enumString == "Orruk Banner")
    {
        return OrrukBanner;
    }
    else if (enumString == "Skull Icon")
    {
        return SkullIcon;
    }
    else if (enumString == "None")
    {
        return None;
    }
    return 0;
}

int Orruks::chargeModifier() const
{
    auto modifier = Unit::chargeModifier();
    if (m_waaaghDrummer)
        modifier += 2;
    return modifier;
}

int Orruks::braveryModifier() const
{
    auto modifier = Unit::braveryModifier();
    if (m_standardBearer == OrrukBanner)
    {
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
        if (!units.empty())
        {
            modifier += 2;
        }
    }
    return modifier;
}

Rerolls Orruks::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Choppas
    if ((m_weapons == PairedChoppas) && (weapon->name() == m_choppa.name()))
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, target);
}

int Orruks::extraAttacks(const Weapon *weapon) const
{
    auto attacks = Unit::extraAttacks(weapon);
    // Mob Rule
    if ((remainingModels() >= 20) && !weapon->isMissile())
    {
        attacks += 1;
    }
    return attacks;
}

Rerolls Orruks::toSaveRerolls(const Weapon *weapon) const
{
    // Waaagh! Shield
    if (!weapon->isMissile() && (m_weapons == ChoppaAndShield || m_weapons == SpearAndShield))
    {
        return RerollFailed;
    }
    return Unit::toSaveRerolls(weapon);
}

int Orruks::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto modifier = Unit::toHitModifier(weapon, target);

    // Ready Boyz! Aim! Fire!
    if (weapon->name() == m_orrukBows.name())
    {
        if (!m_moved)
        {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(m_owningPlayer), 3.0f);
            if (units.empty())
            {
                modifier += 1;
            }
        }
    }
    return modifier;
}

} // namespace Greenskinz
