/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlagueMonks.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Skaven
{

static FactoryMethod factoryMethod = {
    PlagueMonks::Create,
    PlagueMonks::ValueToString,
    PlagueMonks::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = PlagueMonks::MIN_UNIT_SIZE}, PlagueMonks::MIN_UNIT_SIZE,
            PlagueMonks::MAX_UNIT_SIZE, PlagueMonks::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", {.m_intValue = PlagueMonks::PairedFoetidBlades}, PlagueMonks::PairedFoetidBlades,
            PlagueMonks::FoetidBladeAndWoeStave, 1
        },
        {ParamType::Integer, "Contagion Banners", {.m_intValue = 0}, 0, PlagueMonks::MAX_UNIT_SIZE/PlagueMonks::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Icons Of Pestilence", {.m_intValue = 0}, 0, PlagueMonks::MAX_UNIT_SIZE/PlagueMonks::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Doom Gongs", {.m_intValue = 0}, 0, PlagueMonks::MAX_UNIT_SIZE/PlagueMonks::MIN_UNIT_SIZE, 1},
        {ParamType::Integer, "Bale Chimes", {.m_intValue = 0}, 0, PlagueMonks::MAX_UNIT_SIZE/PlagueMonks::MIN_UNIT_SIZE, 1}
    },
    CHAOS,
    SKAVEN
};

bool PlagueMonks::s_registered = false;

PlagueMonks::PlagueMonks() :
    Skaventide("Plague Monks", 6, WOUNDS, 5, 6, false),
    m_foetidBlade(Weapon::Type::Melee, "Foetid Blade", 1, 2, 4, 4, 0, 1),
    m_woeStave(Weapon::Type::Melee, "Woe-stave", 2, 1, 4, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, PLAGUE_MONKS};
}

bool PlagueMonks::configure(int numModels, WeaponOptions weapons, int contagionBanners, int iconsOfPestilence, int doomGongs, int baleChimes)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    int maxBanners = numModels / MIN_UNIT_SIZE;
    int maxHarbingers = numModels / MIN_UNIT_SIZE;
    if (contagionBanners + iconsOfPestilence > maxBanners)
    {
        return false;
    }
    if (doomGongs + baleChimes > maxHarbingers)
    {
        return false;
    }

    m_weapons = weapons;
    m_numContagionBanners = contagionBanners;
    m_numIconsOfPestilence = iconsOfPestilence;
    m_numDoomGongs = doomGongs;
    m_numBaleChimes = baleChimes;

    Model bringer(BASESIZE, WOUNDS);
    bringer.addMeleeWeapon(&m_foetidBlade);
    addModel(bringer);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == PairedFoetidBlades)
        {
            model.addMeleeWeapon(&m_foetidBlade);
        }
        else if (weapons == FoetidBladeAndWoeStave)
        {
            model.addMeleeWeapon(&m_foetidBlade);
            model.addMeleeWeapon(&m_woeStave);
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

void PlagueMonks::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_foetidBlade);
    visitor(&m_woeStave);
}

Unit *PlagueMonks::Create(const ParameterList &parameters)
{
    auto unit = new PlagueMonks();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapons = (WeaponOptions)GetEnumParam("Weapons", parameters, PairedFoetidBlades);
    int contagionBanners = GetIntParam("Contagion Banners", parameters, 0);
    int iconsOfPestilence = GetIntParam("Icons Of Pestilence", parameters, 0);
    int doomGongs = GetIntParam("Doom Gongs", parameters, 0);
    int baleChimes = GetIntParam("Bale Chimes", parameters, 0);

    bool ok = unit->configure(numModels, weapons, contagionBanners, iconsOfPestilence, doomGongs, baleChimes);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void PlagueMonks::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Plague Monks", factoryMethod);
    }
}

Wounds PlagueMonks::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Icon of Pestilence
    if ((woundRoll == 6) && (m_numIconsOfPestilence > 0) && !weapon->isMissile())
    {
        return {weapon->damage()+1, 0};
    }
    return Skaventide::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int PlagueMonks::runModifier() const
{
    // Doom Gongs
    int modifier = Skaventide::runModifier();
    if (m_numDoomGongs > 0)
        modifier += 1;
    return modifier;
}

int PlagueMonks::chargeModifier() const
{
    // Doom Gongs
    int modifier = Skaventide::chargeModifier();
    if (m_numDoomGongs > 0)
        modifier += 1;
    return modifier;
}

Rerolls PlagueMonks::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Pair of Foetid Blades
    if ((m_weapons == PairedFoetidBlades) && (weapon->name() == m_foetidBlade.name()))
    {
        return RerollFailed;
    }
    return Skaventide::toHitRerolls(weapon, target);
}

int PlagueMonks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = Skaventide::extraAttacks(nullptr, weapon, target);
    // Frenzied Assault
    if (m_charged && !weapon->isMissile())
    {
        attacks += 1;
    }
    return attacks;
}

void PlagueMonks::onStartHero(PlayerId player)
{
    if (m_owningPlayer == player)
    {
        // Book of Woes
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(player));
        if (distanceTo(unit) <= 13.0f && !unit->hasKeyword(CLANS_PESTILENS))
        {
            Dice dice;
            int roll = dice.rollD6();
            if (roll == 6)
            {
                unit->applyDamage({0, dice.rollD3()});
            }
            else if (roll >= 4)
            {
                unit->applyDamage({0, 1});
            }
        }
    }
}

std::string PlagueMonks::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == PairedFoetidBlades)
        {
            return "Paired Foetid Blades";
        }
        else if (parameter.m_intValue == FoetidBladeAndWoeStave)
        {
            return "Foetid Blade And Woe Stave";
        }
    }

    return ParameterValueToString(parameter);
}

int PlagueMonks::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Paired Foetid Blades")
    {
        return PairedFoetidBlades;
    }
    else if (enumString == "Foetid Blade And Woe Stave")
    {
        return FoetidBladeAndWoeStave;
    }
    return 0;
}

} // namespace Skaven
