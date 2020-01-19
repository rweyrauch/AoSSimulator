/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/HorrorsOfTzeentch.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Tzeentch
{
static FactoryMethod factoryMethod = {
    HorrorsOfTzeentch::Create,
    TzeentchBase::ValueToString,
    TzeentchBase::EnumStringToInt,
    HorrorsOfTzeentch::ComputePoints,
    {
        {
            ParamType::Integer, "Models", HorrorsOfTzeentch::MIN_UNIT_SIZE, HorrorsOfTzeentch::MIN_UNIT_SIZE,
            HorrorsOfTzeentch::MAX_UNIT_SIZE, HorrorsOfTzeentch::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblower", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None, TzeentchBase::GuildOfSummoners, 1},
    },
    CHAOS,
    { TZEENTCH }
};

bool HorrorsOfTzeentch::s_registered = false;

HorrorsOfTzeentch::HorrorsOfTzeentch() :
    TzeentchBase("Horrors of Tzeentch", 5, WOUNDS, 10, 6, false),
    m_magicalFlamesPink(Weapon::Type::Missile, "Magical Flames (Pink)", 12, 3, 5, 4, 0, 1),
    m_magicalFlamesBlue(Weapon::Type::Missile, "Magical Flames (Blue)", 12, 2, 5, 4, 0, 1),
    m_magicalFlamesBrimstone(Weapon::Type::Missile, "Magical Flames (Brimstone)", 12, 1, 5, 4, 0, 1),
    m_talonedHandsPink(Weapon::Type::Melee, "Taloned Hands (Pink)",  1, 1, 5, 4, 0, 1),
    m_talonedHandsBlue(Weapon::Type::Melee, "Taloned Hands (Blue)",  1, 1, 5, 4, 0, 1),
    m_talonedHandsBrimstone(Weapon::Type::Melee, "Taloned Hands (Brimstone)",  1, 2, 5, 4, 0, 1),
    m_talonedHandsIridescent(Weapon::Type::Melee, "Taloned Hands (Iridescent)", 1, 2, 5, 4, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, TZEENTCH, HORROR, HORROR_OF_TZEENTCH};
    m_weapons = {&m_magicalFlamesPink,
                 &m_magicalFlamesBlue,
                 &m_magicalFlamesBrimstone,
                 &m_talonedHandsPink,
                 &m_talonedHandsBlue,
                 &m_talonedHandsBrimstone,
                 &m_talonedHandsIridescent};

    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool HorrorsOfTzeentch::configure(int numModels, bool iconBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_hornblower = hornblower;

    auto horror = new Model(BASESIZE, WOUNDS);
    horror->addMissileWeapon(&m_magicalFlamesPink);
    horror->addMeleeWeapon(&m_talonedHandsIridescent);
    addModel(horror);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_magicalFlamesPink);
        model->addMeleeWeapon(&m_talonedHandsPink);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

Unit *HorrorsOfTzeentch::Create(const ParameterList &parameters)
{
    auto unit = new HorrorsOfTzeentch();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    auto coven = (ChangeCoven)GetEnumParam("Change Coven", parameters, TzeentchBase::None);
    unit->setChangeCoven(coven);

    bool ok = unit->configure(numModels, iconBearer, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void HorrorsOfTzeentch::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Horrors of Tzeentch", factoryMethod);
    }
}

void  HorrorsOfTzeentch::computeBattleshockEffect(int roll, int& numFled, int& numAdded) const
{
    Unit::computeBattleshockEffect(roll, numFled, numAdded);
    if (m_iconBearer)
    {
        // Icon Bearer
        if (roll == 1)
        {
            Dice dice;
            numAdded = dice.rollD6();
        }
    }
}

void HorrorsOfTzeentch::restoreModels(int numModels)
{
    // Icon Bearer
    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_magicalFlamesPink);
        model->addMeleeWeapon(&m_talonedHandsPink);
        addModel(model);
    }
}

int HorrorsOfTzeentch::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);
    // Flickering Flames
    if (remainingModels() >= 20)
        modifier += 1;
    return modifier;
}

int HorrorsOfTzeentch::castingModifier() const
{
    int modifier = Unit::castingModifier();

    // Locus of Conjuration
    auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 9.0f);
    for (auto ip : units)
    {
        if (ip->hasKeyword(TZEENTCH) && ip->hasKeyword(DAEMON) && ip->hasKeyword(HERO))
        {
            modifier+=1;
            break;
        }
    }
    return modifier;
}

int HorrorsOfTzeentch::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //namespace Tzeentch