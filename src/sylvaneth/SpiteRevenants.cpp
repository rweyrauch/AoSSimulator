/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/SpiteRevenants.h>
#include <UnitFactory.h>

namespace Sylvaneth {

static FactoryMethod factoryMethod = {
    SpiteRevenants::Create,
    {
        {ParamType::Integer, "numModels", {.m_intValue = SpiteRevenants::MIN_UNIT_SIZE}, SpiteRevenants::MIN_UNIT_SIZE,
         SpiteRevenants::MAX_UNIT_SIZE},
    }
};

bool SpiteRevenants::s_registered = false;

Weapon SpiteRevenants::s_cruelTalonsAndFangs(Weapon::Type::Melee, "Cruel Talons and Fangs", 1, 3, 4, 4, 0, 1);
Weapon SpiteRevenants::s_cruelTalonsAndFangsShadestalker(Weapon::Type::Melee, "Cruel Talons and Fangs", 1, 4, 4, 4, 0, 1);

SpiteRevenants::SpiteRevenants() :
    Unit("Spite Revenants", 5, WOUNDS, 6, 5, false)
{
    m_keywords = {ORDER, SYLVANETH, SPITE_REVENANTS };
}

bool SpiteRevenants::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
        return false;

    Model shadestalker(BASESIZE, WOUNDS);
    shadestalker.addMeleeWeapon(&s_cruelTalonsAndFangsShadestalker);
    addModel(shadestalker);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_cruelTalonsAndFangs);
        addModel(model);
    }
    return true;
}

Unit *SpiteRevenants::Create(const ParameterList &parameters)
{
    auto unit = new SpiteRevenants();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void SpiteRevenants::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Spite-Revenants", factoryMethod);
    }
}

} // namespace Sylvaneth