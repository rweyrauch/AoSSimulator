/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/IshlaenGuard.h>
#include <UnitFactory.h>

namespace IdonethDeepkin
{

bool AkhelianIshlaenGuard::s_registered = false;

static FactoryMethod factoryMethod = {
    AkhelianIshlaenGuard::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", AkhelianIshlaenGuard::MIN_UNIT_SIZE, AkhelianIshlaenGuard::MIN_UNIT_SIZE,
            AkhelianIshlaenGuard::MAX_UNIT_SIZE, AkhelianIshlaenGuard::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Standard Bearers", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Musicians", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
    },
    ORDER,
    IDONETH_DEEPKIN
};


AkhelianIshlaenGuard::AkhelianIshlaenGuard() :
    Unit("Akhelian Ishlaen Guard", 14, WOUNDS, 6, 4, true),
    m_helsabre(Weapon::Type::Melee, "Helsabre", 1, 3, 3, 3, 0, 1),
    m_helsabrePrince(Weapon::Type::Melee, "Helsabre (Prince)", 1, 4, 3, 3, 0, 1),
    m_fangmoraFangedMaw(Weapon::Type::Melee, "Fangmora's Fanged Maw", 1, 1, 3, 3, 0, RAND_D3),
    m_fangmoraLashingTail(Weapon::Type::Melee, "Fangmora's Lashing Tail", 2, RAND_D3, 3, 3, 0, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, AKHELIAN, AKHELIAN_GUARD, ISHLAEN_GUARD};
}

bool AkhelianIshlaenGuard::configure(int numModels, bool standardBearers, bool musicians)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearers = standardBearers;
    m_musicians = musicians;

    Model prince(BASESIZE, WOUNDS);
    prince.addMeleeWeapon(&m_helsabrePrince);
    prince.addMeleeWeapon(&m_fangmoraFangedMaw);
    prince.addMeleeWeapon(&m_fangmoraLashingTail);
    addModel(prince);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_helsabre);
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

void AkhelianIshlaenGuard::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_helsabre);
    visitor(&m_helsabrePrince);
    visitor(&m_fangmoraFangedMaw);
    visitor(&m_fangmoraLashingTail);
}

Unit *AkhelianIshlaenGuard::Create(const ParameterList &parameters)
{
    auto unit = new AkhelianIshlaenGuard();
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

void AkhelianIshlaenGuard::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Akhelian Ishlaen Guard", factoryMethod);
    }
}

Rerolls AkhelianIshlaenGuard::battleshockRerolls() const
{
    if (m_standardBearers)
    { return RerollFailed; }
    return Unit::battleshockRerolls();
}

Rerolls AkhelianIshlaenGuard::chargeRerolls() const
{
    if (m_musicians)
    { return RerollFailed; }
    return Unit::chargeRerolls();
}

int AkhelianIshlaenGuard::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);

    // Biovoltaic Barrier
    if (m_charged)
    {
        modifier += 1;
    }

    // Biovoltaic Barrier - ignore rend by cancelling it out.
    if (weapon->rend() < 0)
    {
        modifier = -weapon->rend();
    }

    return modifier;
}

} // namespace IdonethDeepkin