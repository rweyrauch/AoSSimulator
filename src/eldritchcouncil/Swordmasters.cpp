/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <eldritchcouncil/Swordmasters.h>
#include <UnitFactory.h>

namespace EldritchCouncil
{
static FactoryMethod factoryMethod = {
    Swordmasters::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "Models", Swordmasters::MIN_UNIT_SIZE, Swordmasters::MIN_UNIT_SIZE,
            Swordmasters::MAX_UNIT_SIZE, Swordmasters::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Standard Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Hornblower", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
    },
    ORDER,
    ELDRITCH_COUNCIL
};

bool Swordmasters::s_registered = false;

Swordmasters::Swordmasters() :
    Unit("Swordmasters", 6, WOUNDS, 7, 4, false),
    m_greatsword(Weapon::Type::Melee, "Greatsword", 1, 2, 3, 3, -1, 1),
    m_greatswordLord(Weapon::Type::Melee, "Greatsword", 1, 3, 3, 3, -1, 1)
{
    m_keywords = {ORDER, AELF, ELDRITCH_COUNCIL, SWORDMASTERS};
}

bool Swordmasters::configure(int numModels, bool hornblower, bool standardBearer)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_standardBearer = standardBearer;
    m_hornblower = hornblower;

    Model lord(BASESIZE, WOUNDS);
    lord.addMeleeWeapon(&m_greatswordLord);
    addModel(lord);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_greatsword);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Swordmasters::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_greatsword);
    visitor(&m_greatswordLord);
}

Unit *Swordmasters::Create(const ParameterList &parameters)
{
    auto unit = new Swordmasters();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
    bool hornblower = GetBoolParam("Hornblower", parameters, false);

    bool ok = unit->configure(numModels, hornblower, standardBearer);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Swordmasters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Swordmasters", factoryMethod);
    }
}

Rerolls Swordmasters::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // A Blur of Blades
    return RerollOnes;
}

Rerolls Swordmasters::toSaveRerolls(const Weapon *weapon) const
{
    // Deflect Shots
    if (weapon->isMissile())
    {
        return RerollFailed;
    }
    return Unit::toSaveRerolls(weapon);
}

} //namespace EldritchCouncil