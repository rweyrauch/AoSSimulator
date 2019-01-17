/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/BoingrotBounderz.h>
#include <UnitFactory.h>

namespace GloomspiteGitz
{
static FactoryMethod factoryMethod = {
        BoingrotBounderz::Create,
        {
                {ParamType::Integer, "numModels", BoingrotBounderz::MIN_UNIT_SIZE, BoingrotBounderz::MIN_UNIT_SIZE, BoingrotBounderz::MAX_UNIT_SIZE}
        }
};

bool BoingrotBounderz::s_registered = false;

Weapon BoingrotBounderz::s_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1);
Weapon BoingrotBounderz::s_pokinLance(Weapon::Type::Melee, "Pokin' Lance", 2, 2, 4, 4, -1, 1);
Weapon BoingrotBounderz::s_pokinLanceBoss(Weapon::Type::Melee, "Pokin' Lance", 2, 2, 3, 4, -1, 1);

BoingrotBounderz::BoingrotBounderz() :
    Unit("Boingrot Bounderz", RAND_2D6, WOUNDS, 5, 4, true)
{
    m_keywords = { DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, BOINGROT_BOUNDERZ };
}

bool BoingrotBounderz::configure(int numModels)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    // Add the boss
    Model boss(BASESIZE, WOUNDS);
    boss.addMeleeWeapon(&s_pokinLanceBoss);
    boss.addMeleeWeapon(&s_fangFilledGob);
    addModel(boss);

    // and the rest
    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(&s_pokinLance);
        model.addMeleeWeapon(&s_fangFilledGob);
        addModel(model);
    }

    return true;
}

Unit *BoingrotBounderz::Create(const ParameterList &parameters)
{
    auto unit = new BoingrotBounderz();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void BoingrotBounderz::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Boingrot Bounderz", factoryMethod);
    }
}

} // namespace GloomspiteGitz
