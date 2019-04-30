/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Ungors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    Ungors::Create,
    Ungors::ValueToString,
    Ungors::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Ungors::MIN_UNIT_SIZE, Ungors::MIN_UNIT_SIZE,
            Ungors::MAX_UNIT_SIZE, Ungors::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", Ungors::UngorBlade, Ungors::UngorBlade,
            Ungors::GnarledShortspear, 1
        },
        {ParamType::Boolean, "Brayhorn", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool Ungors::s_registered = false;

Ungors::Ungors() :
    BeastsOfChaosBase("Ungors", 6, WOUNDS, 4, 6, false),
    m_ungorBlade(Weapon::Type::Melee, "Ungor Blade", 1, 1, 4, 4, 0, 1),
    m_ungorBladeHalfhorn(Weapon::Type::Melee, "Ungor Blade (Halfhorn)", 1, 2, 4, 4, 0, 1),
    m_gnarledShortspear(Weapon::Type::Melee, "Gnarled Shortspear", 2, 1, 5, 4, 0, 1),
    m_gnarledShortspearHalfhorn(Weapon::Type::Melee, "Gnarled Shortspear (Halfhorn)", 2, 2, 5, 4, 0, 1)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, BRAYHERD, UNGORS};
}

bool Ungors::configure(int numModels, WeaponOptions weapons,
                       bool brayhorn, bool bannerBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_brayhorn = brayhorn;
    m_bannerBearer = bannerBearer;

    m_runAndCharge = m_brayhorn;

    Model halfhorn(BASESIZE, WOUNDS);
    if (weapons == UngorBlade)
    {
        halfhorn.addMeleeWeapon(&m_ungorBladeHalfhorn);
    }
    else if (weapons == GnarledShortspear)
    {
        halfhorn.addMeleeWeapon(&m_gnarledShortspearHalfhorn);
    }
    addModel(halfhorn);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == UngorBlade)
        {
            model.addMeleeWeapon(&m_ungorBlade);
        }
        else if (weapons == GnarledShortspear)
        {
            model.addMeleeWeapon(&m_gnarledShortspear);
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

void Ungors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_ungorBlade);
    visitor(&m_ungorBladeHalfhorn);
    visitor(&m_gnarledShortspear);
    visitor(&m_gnarledShortspearHalfhorn);
}

Unit *Ungors::Create(const ParameterList &parameters)
{
    auto unit = new Ungors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapon = (WeaponOptions) GetEnumParam("Weapons", parameters, UngorBlade);
    bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
    bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

    bool ok = unit->configure(numModels, weapon, brayhorn, bannerBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Ungors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Ungors", factoryMethod);
    }
}

std::string Ungors::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == UngorBlade) { return "Ungor Blade"; }
        else if (parameter.m_intValue == GnarledShortspear) { return "Gnarled Shortspear"; }
    }
    return ParameterValueToString(parameter);
}

int Ungors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Ungor Blade") { return UngorBlade; }
    else if (enumString == "Gnarled Shortspear") { return GnarledShortspear; }
    return 0;
}

Rerolls Ungors::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Braying Hatred
    if (remainingModels() >= 30)
    {
        return RerollOnesAndTwos;
    }
    else if (remainingModels() >= 20)
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, target);
}

int Ungors::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);
    // Half-shields
    if (!weapon->isMissile())
    {
        modifier += 1;
    }
    return modifier;
}

} // namespace BeastsOfChaos