/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlagueCenserBearers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Skaven
{
bool PlagueCenserBearers::s_registered = false;

Unit *PlagueCenserBearers::Create(const ParameterList &parameters)
{
    auto unit = new PlagueCenserBearers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int PlagueCenserBearers::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

void PlagueCenserBearers::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Create,
            Skaventide::ValueToString,
            Skaventide::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE,MAX_UNIT_SIZE, MIN_UNIT_SIZE},
            },
            CHAOS,
            { SKAVEN }
        };

        s_registered = UnitFactory::Register("Plague Censer Bearers", factoryMethod);
    }
}

PlagueCenserBearers::PlagueCenserBearers() :
    Skaventide("Plague Censer Bearers", 6, WOUNDS, 5, 6, false),
    m_censer(Weapon::Type::Melee, "Plague Censer", 2, 2, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, PLAGUE_CENSER_BEARERS};
    m_weapons = {&m_censer};
}

bool PlagueCenserBearers::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    for (auto i = 0; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_censer);
        addModel(model);
    }
    m_points = ComputePoints(numModels);

    return true;
}

int PlagueCenserBearers::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    auto extra = Skaventide::extraAttacks(attackingModel, weapon, target);

    // Frenzied Assault
    if (m_charged) extra++;

    return extra;
}

Rerolls PlagueCenserBearers::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Plaque Disciples
    auto monks = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), PLAGUE_MONKS, 18.0f);
    if (monks) return RerollFailed;

    return Unit::toHitRerolls(weapon, target);
}

Rerolls PlagueCenserBearers::battleshockRerolls() const
{
    // Plaque Disciples
    auto monks = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), PLAGUE_MONKS, 18.0f);
    if (monks) return RerollFailed;

    return Unit::battleshockRerolls();
}

Wounds PlagueCenserBearers::onEndCombat(PlayerId player)
{
    Wounds wounds = Unit::onEndCombat(player);

    // Poisonous Fumes
    auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0f);
    for (auto unit : units)
    {
        if (!unit->hasKeyword(CLANS_PESTILENS))
        {
            int mortalWounds = 0;
            int roll = Dice::rollD6();
            if (roll == 6) mortalWounds = Dice::rollD3();
            else if (roll >= 4) mortalWounds = 1;

            unit->applyDamage({0, mortalWounds});
            wounds.mortal += mortalWounds;
        }
    }
    return wounds;
}

} //namespace Skaven
