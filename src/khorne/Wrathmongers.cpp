/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/Wrathmongers.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Wrathmongers::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Wrathmongers::MIN_UNIT_SIZE},
            Wrathmongers::MIN_UNIT_SIZE,
            Wrathmongers::MAX_UNIT_SIZE, Wrathmongers::MIN_UNIT_SIZE
        },
    },
    CHAOS,
    KHORNE
};

bool Wrathmongers::s_registered = false;

Wrathmongers::Wrathmongers() :
    Unit("Wrathmongers", 5, WOUNDS, 7, 5, false),
    m_wrathflails(Weapon::Type::Melee, "Wrath-flails", 2, 4, 4, 3, -1, 1),
    m_wrathflailsMaster(Weapon::Type::Melee, "Wrath-flails", 2, 5, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, WRATHMONGERS};
}

bool Wrathmongers::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    Model master(BASESIZE, WOUNDS);
    master.addMeleeWeapon(&m_wrathflailsMaster);
    addModel(master);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&m_wrathflails);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Wrathmongers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_wrathflails);
    visitor(&m_wrathflailsMaster);
}

Unit *Wrathmongers::Create(const ParameterList &parameters)
{
    auto unit = new Wrathmongers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Wrathmongers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Wrathmongers", factoryMethod);
    }
}

int Wrathmongers::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    int modifier = Unit::toHitModifier(weapon, target);

    // Wrath-flails
    if (m_charged)
    {
        modifier += 1;
    }
    return modifier;
}

} //namespace Khorne