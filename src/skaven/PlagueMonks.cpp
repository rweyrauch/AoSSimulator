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

bool PlagueMonks::s_registered = false;

PlagueMonks::PlagueMonks() :
    Skaventide("Plague Monks", 6, WOUNDS, 5, 6, false),
    m_foetidBlade(Weapon::Type::Melee, "Foetid Blade", 1, 2, 4, 4, 0, 1),
    m_woeStave(Weapon::Type::Melee, "Woe-stave", 2, 1, 4, 5, 0, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, PLAGUE_MONKS};
    m_weapons = {&m_foetidBlade, &m_woeStave};
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

    m_weaponOption = weapons;
    m_numContagionBanners = contagionBanners;
    m_numIconsOfPestilence = iconsOfPestilence;
    m_numDoomGongs = doomGongs;
    m_numBaleChimes = baleChimes;

    auto bringer = new Model(BASESIZE, WOUNDS);
    bringer->addMeleeWeapon(&m_foetidBlade);
    addModel(bringer);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (weapons == PairedFoetidBlades)
        {
            model->addMeleeWeapon(&m_foetidBlade);
        }
        else if (weapons == FoetidBladeAndWoeStave)
        {
            model->addMeleeWeapon(&m_foetidBlade);
            model->addMeleeWeapon(&m_woeStave);
        }
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
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
        static auto factoryMethod = new FactoryMethod{
            Create,
            ValueToString,
            EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                {ParamType::Enum, "Weapons", PairedFoetidBlades, PairedFoetidBlades, FoetidBladeAndWoeStave, 1},
                {ParamType::Integer, "Contagion Banners", 0, 0, MAX_UNIT_SIZE/MIN_UNIT_SIZE, 1},
                {ParamType::Integer, "Icons Of Pestilence", 0, 0, MAX_UNIT_SIZE/MIN_UNIT_SIZE, 1},
                {ParamType::Integer, "Doom Gongs", 0, 0, MAX_UNIT_SIZE/MIN_UNIT_SIZE, 1},
                {ParamType::Integer, "Bale Chimes", 0, 0, MAX_UNIT_SIZE/MIN_UNIT_SIZE, 1}
            },
            CHAOS,
            { SKAVEN }
        };
        s_registered = UnitFactory::Register("Plague Monks", *factoryMethod);
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
    if ((m_weaponOption == PairedFoetidBlades) && (weapon->name() == m_foetidBlade.name()))
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
    if (owningPlayer() == player)
    {
        // Book of Woes
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(player));
        if (distanceTo(unit) <= 13.0f && !unit->hasKeyword(CLANS_PESTILENS))
        {
            int roll = Dice::rollD6();
            if (roll == 6)
            {
                unit->applyDamage({0, Dice::rollD3()});
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
    if (std::string(parameter.name) == "Weapons")
    {
        if (parameter.intValue == PairedFoetidBlades)
        {
            return "Paired Foetid Blades";
        }
        else if (parameter.intValue == FoetidBladeAndWoeStave)
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

int PlagueMonks::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int PlagueMonks::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Bale-chime
    if ((m_numBaleChimes > 0) && (hitRoll == 6) && !weapon->isMissile())
    {
        return weapon->rend()-1;
    }
    return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
}

} // namespace Skaven
