/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/MorrsarrGuard.h>
#include <UnitFactory.h>

namespace IdonethDeepkin
{

static FactoryMethod factoryMethod = {
    AkhelianMorrsarrGuard::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = AkhelianMorrsarrGuard::MIN_UNIT_SIZE}, AkhelianMorrsarrGuard::MIN_UNIT_SIZE,
            AkhelianMorrsarrGuard::MAX_UNIT_SIZE, AkhelianMorrsarrGuard::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Standard Bearers", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "Musicians", {.m_boolValue = true}, false, false},
    },
    ORDER,
    IDONETH_DEEPKIN
};


bool AkhelianMorrsarrGuard::s_registered = false;

AkhelianMorrsarrGuard::AkhelianMorrsarrGuard() :
    Unit("Akhelian Morrsarr Guard", 14, WOUNDS, 6, 4, true),
    m_voltspear(Weapon::Type::Melee, "Voltspear", 2, 2, 3, 3, 0, 1),
    m_voltspearPrince(Weapon::Type::Melee, "Voltspear (Prince)", 2, 3, 3, 3, 0, 1),
    m_fangmoraFangedMaw(Weapon::Type::Melee, "Fangmora's Fanged Maw", 1, 1, 3, 3, 0, RAND_D3),
    m_fangmoraLashingTail(Weapon::Type::Melee, "Fangmora's Lashing Tail", 2, RAND_D3, 3, 3, 0, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, AKHELIAN, AKHELIAN_GUARD, MORRSARR_GUARD};
}

bool AkhelianMorrsarrGuard::configure(int numModels, bool standardBearers, bool musicians)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearers = standardBearers;
    m_musicians = musicians;

    Model prince(BASESIZE, WOUNDS);
    prince.addMeleeWeapon(&m_voltspearPrince);
    prince.addMeleeWeapon(&m_fangmoraFangedMaw);
    prince.addMeleeWeapon(&m_fangmoraLashingTail);
    addModel(prince);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_voltspear);
        model.addMeleeWeapon(&m_fangmoraFangedMaw);
        model.addMeleeWeapon(&m_fangmoraLashingTail);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void AkhelianMorrsarrGuard::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_voltspear);
    visitor(&m_voltspearPrince);
    visitor(&m_fangmoraFangedMaw);
    visitor(&m_fangmoraLashingTail);
}

Unit *AkhelianMorrsarrGuard::Create(const ParameterList &parameters)
{
    auto unit = new AkhelianMorrsarrGuard();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standardBearers = GetBoolParam("Standard Bearers", parameters, true);
    bool musicians = GetBoolParam("Musicians", parameters, true);

    bool ok = unit->configure(numModels, standardBearers, musicians);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void AkhelianMorrsarrGuard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Akhelian Morrsarr Guard", factoryMethod);
    }
}

Rerolls AkhelianMorrsarrGuard::battleshockRerolls() const
{
    if (m_standardBearers)
    { return RerollFailed; }
    return Unit::battleshockRerolls();
}

Rerolls AkhelianMorrsarrGuard::chargeRerolls() const
{
    if (m_musicians)
    { return RerollFailed; }
    return Unit::chargeRerolls();
}

void AkhelianMorrsarrGuard::onBeginTurn(int battleRound)
{
    Unit::onBeginTurn(battleRound);

    // reset weapon damage and rend (if previously charged)
    m_voltspear.setDamage(m_voltspear.damage());
    m_voltspear.setRend(m_voltspear.rend());
    m_voltspearPrince.setDamage(m_voltspearPrince.damage());
    m_voltspearPrince.setRend(m_voltspearPrince.rend());
}

void AkhelianMorrsarrGuard::onCharged()
{
    Unit::onCharged();

    // Wave Riders
    m_voltspear.setDamage(2);
    m_voltspear.setRend(-2);
    m_voltspearPrince.setDamage(2);
    m_voltspearPrince.setRend(-2);
}

void AkhelianMorrsarrGuard::onStartCombat(PlayerId player)
{
    Unit::onStartCombat(player);

    if (player != owningPlayer())
    { return; }

    // Biovoltaic Blast
    if (m_meleeTarget && !m_usedBiovoltaicBlast)
    {
        Dice dice;
        Dice::RollResult roll;
        dice.rollD6(m_meleeTarget->remainingModels(), roll);
        int totalMortalWounds = 0;
        int num6s = roll.numUnmodified6s();
        for (int i = 0; i < roll.numUnmodified6s(); i++)
        {
            totalMortalWounds += dice.rollD3();
        }
        int num3plus = roll.rollsGE(3);
        totalMortalWounds += (num3plus - num6s);

        m_meleeTarget->applyDamage({0, totalMortalWounds});

        m_usedBiovoltaicBlast = true;
    }

}

} // namespace IdonethDeepkin