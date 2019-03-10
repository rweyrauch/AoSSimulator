/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Bestigors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    Bestigors::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = Bestigors::MIN_UNIT_SIZE}, Bestigors::MIN_UNIT_SIZE,
            Bestigors::MAX_UNIT_SIZE, Bestigors::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Brayhorn", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "Banner Bearer", {.m_boolValue = true}, false, false}
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool Bestigors::s_registered = false;

Bestigors::Bestigors() :
    Unit("Bestigors", 6, WOUNDS, 6, 4, false),
    m_despoilerAxe(Weapon::Type::Melee, "Despoiler Axe", 1, 2, 4, 3, -1, 1),
    m_despoilerAxeGougeHorn(Weapon::Type::Melee, "Despoiler Axe (Gouge-horn)", 1, 3, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, BESTIGORS};
}

bool Bestigors::configure(int numModels, bool brayhorn, bool bannerBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_brayhorn = brayhorn;
    m_bannerBearer = bannerBearer;

    m_runAndCharge = m_brayhorn;

    Model gougehorn(BASESIZE, WOUNDS);
    gougehorn.addMeleeWeapon(&m_despoilerAxeGougeHorn);
    addModel(gougehorn);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_despoilerAxe);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Bestigors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_despoilerAxe);
    visitor(&m_despoilerAxeGougeHorn);
}

Unit *Bestigors::Create(const ParameterList &parameters)
{
    auto unit = new Bestigors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
    bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

    bool ok = unit->configure(numModels, brayhorn, bannerBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bestigors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bestigors", factoryMethod);
    }
}

int Bestigors::toHitModifier(const Weapon *weapon, const Unit *unit) const
{
    // Despoilers
    int modifier = Unit::toHitModifier(weapon, unit);
    if (unit->remainingModels() >= 10)
    {
        modifier += 1;
    }
    return modifier;
}

Rerolls Bestigors::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    // Despoilers
    if (unit->hasKeyword(ORDER))
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, unit);
}

int Bestigors::extraAttacks(const Weapon *weapon) const
{
    // Beastial Charge
    int attacks = Unit::extraAttacks(weapon);
    if (m_charged)
    {
        attacks += 1;
    }
    return attacks;
}

int Bestigors::runModifier() const
{
    int modifier = Unit::runModifier();
    if (m_bannerBearer)
    {
        modifier += 1;
    }
    return modifier;
}

} // namespace BeastsOfChaos