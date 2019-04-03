/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Gors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    Gors::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", Gors::MIN_UNIT_SIZE, Gors::MIN_UNIT_SIZE,
            Gors::MAX_UNIT_SIZE, Gors::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Paired Blades", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Brayhorn", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool Gors::s_registered = false;


Gors::Gors() :
    Unit("Gors", 6, WOUNDS, 5, 5, false),
    m_gorBlade(Weapon::Type::Melee, "Gor Blade", 1, 1, 4, 3, 0, 1),
    m_gorBladeFoeRender(Weapon::Type::Melee, "Gor Blade (Foe-render)", 1, 2, 4, 3, 0, 1)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, BRAYHERD, GORS};
}

bool Gors::configure(int numModels, bool pairedBlades, bool brayhorn, bool bannerBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_brayhorn = brayhorn;
    m_bannerBearer = bannerBearer;
    m_pairedBlades = pairedBlades;

    Model foe(BASESIZE, WOUNDS);
    foe.addMeleeWeapon(&m_gorBladeFoeRender);
    addModel(foe);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_gorBlade);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Gors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_gorBlade);
    visitor(&m_gorBladeFoeRender);
}

Unit *Gors::Create(const ParameterList &parameters)
{
    auto unit = new Gors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool pairedBlades = GetBoolParam("Paired Blades", parameters, false);
    bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
    bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

    bool ok = unit->configure(numModels, pairedBlades, brayhorn, bannerBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Gors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Gors", factoryMethod);
    }
}

Rerolls Gors::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Rend and Tear
    if (m_pairedBlades)
    { return RerollOnes; }
    return Unit::toHitRerolls(weapon, target);
}

int Gors::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);
    // Beastsheilds
    if (!m_pairedBlades)
    { modifier += 1; }
    return modifier;
}

int Gors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const
{
    int attacks = Unit::extraAttacks(nullptr, weapon, target);
    // Anarchy and Mayhem
    if (remainingModels() >= 20)
    {
        attacks += 1;
    }
    return attacks;
}

} // namespace BeastsOfChaos