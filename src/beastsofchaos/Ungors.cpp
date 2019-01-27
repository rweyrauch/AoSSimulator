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
        {ParamType::Integer, "numModels", {.m_intValue = Ungors::MIN_UNIT_SIZE}, Ungors::MIN_UNIT_SIZE,
         Ungors::MAX_UNIT_SIZE, Ungors::MIN_UNIT_SIZE},
        {ParamType::Integer, "weapons", {.m_intValue = Ungors::UngorBlade}, Ungors::UngorBlade,
         Ungors::GnarledShortspear, 1},
        {ParamType::Boolean, "brayhorn", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "bannerBearer", {.m_boolValue = true}, false, false}
    }
};

bool Ungors::s_registered = false;

Weapon Ungors::s_ungorBlade(Weapon::Type::Melee, "Ungor Blade", 1, 1, 4, 4, 0, 1);
Weapon Ungors::s_ungorBladeHalfhorn(Weapon::Type::Melee, "Ungor Blade (Halfhorn)", 1, 2, 4, 4, 0, 1);
Weapon Ungors::s_gnarledShortspear(Weapon::Type::Melee, "Gnarled Shortspear", 2, 1, 5, 4, 0, 1);
Weapon Ungors::s_gnarledShortspearHalfhorn(Weapon::Type::Melee, "Gnarled Shortspear (Halfhorn)", 2, 2, 5, 4, 0, 1);

Ungors::Ungors() :
    Unit("Ungors", 6, WOUNDS, 4, 6, false)
{
    m_keywords = { CHAOS, BEASTS_OF_CHAOS, BRAYHERD, UNGORS};
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
        halfhorn.addMeleeWeapon(&s_ungorBladeHalfhorn);
    else if (weapons == GnarledShortspear)
        halfhorn.addMeleeWeapon(&s_gnarledShortspearHalfhorn);
    addModel(halfhorn);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (weapons == UngorBlade)
            model.addMeleeWeapon(&s_ungorBlade);
        else if (weapons == GnarledShortspear)
            model.addMeleeWeapon(&s_gnarledShortspear);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
        m_points = POINTS_MAX_UNIT_SIZE;

    return true;
}

void Ungors::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&s_ungorBlade);
    visitor(&s_ungorBladeHalfhorn);
    visitor(&s_gnarledShortspear);
    visitor(&s_gnarledShortspearHalfhorn);
}

Unit *Ungors::Create(const ParameterList &parameters)
{
    auto unit = new Ungors();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    auto weapon = (WeaponOptions)GetIntParam("weapons", parameters, UngorBlade);
    bool brayhorn = GetBoolParam("brayhorn", parameters, false);
    bool bannerBearer = GetBoolParam("bannerBearer", parameters, false);

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
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == UngorBlade) return "UngorBlade";
        else if (parameter.m_intValue == GnarledShortspear) return "GnarledShortspear";
    }
    return ParameterValueToString(parameter);
}

int Ungors::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "UngorBlade") return UngorBlade;
    else if (enumString == "GnarledShortspear") return GnarledShortspear;
    return 0;
}

} // namespace BeastsOfChaos