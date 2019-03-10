/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Bullgors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos
{
static FactoryMethod factoryMethod = {
    Bullgors::Create,
    Bullgors::ValueToString,
    Bullgors::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", {.m_intValue = Bullgors::MIN_UNIT_SIZE}, Bullgors::MIN_UNIT_SIZE,
            Bullgors::MAX_UNIT_SIZE, Bullgors::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", {.m_intValue = Bullgors::BullgorAxe}, Bullgors::BullgorAxe,
            Bullgors::BullgorGreatAxe, 1
        },
        {ParamType::Boolean, "Drummer", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "Banner Bearer", {.m_boolValue = true}, false, false}
    },
    CHAOS,
    BEASTS_OF_CHAOS
};

bool Bullgors::s_registered = false;

Bullgors::Bullgors() :
    Unit("Bullgors", 7, WOUNDS, 6, 5, false),
    m_bullgorHorns(Weapon::Type::Melee, "Bullgor Horns", 1, 2, 4, 4, 0, 1),
    m_bullgorAxe(Weapon::Type::Melee, "Bullgor Axe", 1, 3, 4, 3, -1, 2),
    m_bullgorAxeBloodkine(Weapon::Type::Melee, "Bullgor Axe (Bloodkine)", 1, 4, 4, 3, -1, 2),
    m_bullgorGreatAxe(Weapon::Type::Melee, "Bullgor Great Axe", 1, 2, 4, 3, -2, 3),
    m_bullgorGreatAxeBloodkine(Weapon::Type::Melee, "Bullgor Great Axe (Bloodkine)", 1, 3, 4, 3, -2, 3)
{
    m_keywords = {CHAOS, BEASTS_OF_CHAOS, WARHERD, BULLGORS};
}

bool Bullgors::configure(int numModels, WeaponOptions options,
                         bool drummer, bool bannerBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_drummer = drummer;
    m_bannerBearer = bannerBearer;
    m_pairedAxes = (options == PairedBullgorAxes);
    Model bloodkine(BASESIZE, WOUNDS);
    bloodkine.addMeleeWeapon(&m_bullgorHorns);
    if (options == BullgorAxe || options == PairedBullgorAxes)
    {
        bloodkine.addMeleeWeapon(&m_bullgorAxeBloodkine);
    }
    else if (options == BullgorGreatAxe)
    {
        bloodkine.addMeleeWeapon(&m_bullgorGreatAxeBloodkine);
    }
    addModel(bloodkine);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_bullgorHorns);
        if (options == BullgorAxe || options == PairedBullgorAxes)
        {
            model.addMeleeWeapon(&m_bullgorAxe);
        }
        else if (options == BullgorGreatAxe)
        {
            model.addMeleeWeapon(&m_bullgorGreatAxe);
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

void Bullgors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_bullgorHorns);
    visitor(&m_bullgorAxe);
    visitor(&m_bullgorAxeBloodkine);
    visitor(&m_bullgorGreatAxe);
    visitor(&m_bullgorGreatAxeBloodkine);
}

Unit *Bullgors::Create(const ParameterList &parameters)
{
    auto unit = new Bullgors();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapon = (WeaponOptions) GetEnumParam("Weapons", parameters, BullgorAxe);
    bool drummer = GetBoolParam("Drummer", parameters, false);
    bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

    bool ok = unit->configure(numModels, weapon, drummer, bannerBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bullgors::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bullgors", factoryMethod);
    }
}

std::string Bullgors::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == BullgorAxe) { return "Bullgor Axe"; }
        else if (parameter.m_intValue == PairedBullgorAxes) { return "Paired Bullgor Axes"; }
        else if (parameter.m_intValue == BullgorGreatAxe) { return "Bullgor Great Axe"; }
    }
    return ParameterValueToString(parameter);
}

int Bullgors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Bullgor Axe") { return BullgorAxe; }
    else if (enumString == "Paired Bullgor Axes") { return PairedBullgorAxes; }
    else if (enumString == "Bullgor Great Axe") { return BullgorGreatAxe; }
    return 0;
}

int Bullgors::chargeModifier() const
{
    int modifier = Unit::chargeModifier();
    if (m_drummer)
    {
        modifier += 1;
    }
    return modifier;
}

Rerolls Bullgors::toHitRerolls(const Weapon *weapon, const Unit *unit) const
{
    if (m_pairedAxes)
    {
        return RerollOnes;
    }
    return Unit::toHitRerolls(weapon, unit);
}

int Bullgors::toSaveModifier(const Weapon *weapon) const
{
    int modifier = Unit::toSaveModifier(weapon);
    if (!m_pairedAxes)
    {
        modifier += 1;
    }
    return modifier;
}

Wounds Bullgors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Bloodgreed
    if (woundRoll == 6)
    {
        return {weapon->damage(), 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace BeastsOfChaos