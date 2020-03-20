/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/IronskullsBoyz.h>
#include <UnitFactory.h>

namespace Ironjawz
{
static const int BASESIZE = 32;
static const int WOUNDS = 2;
static const int POINTS_PER_UNIT = 80;

bool IronskullsBoyz::s_registered = false;

IronskullsBoyz::IronskullsBoyz() :
    Ironjawz("Ironskull's Boyz", 4, WOUNDS, 6, 4, false),
    m_eadButt(Weapon::Type::Melee, "'Eadbutt'", 1, 1, 4, 3, 0, RAND_D3),
    m_pairedChoppas(Weapon::Type::Melee, "Pair of Ardboy Choppas", 1, 2, 3, 3, -1, 1),
    m_bigChoppa(Weapon::Type::Melee, "Ardboy Big Choppa", 1, 2, 4, 3, -1, 2)
{
    m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, IRONSUNZ, ARDBOYS, IRONSKULLS_BOYZ};
    m_weapons = {&m_eadButt, &m_pairedChoppas, &m_bigChoppa};
}

bool IronskullsBoyz::configure()
{
    // Add the Boss
    auto bossModel = new Model(BASESIZE, wounds());
    bossModel->addMeleeWeapon(&m_eadButt);
    bossModel->addMeleeWeapon(&m_bigChoppa);
    addModel(bossModel);

    for (auto i = 1; i < 4; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_eadButt);
        model->addMeleeWeapon(&m_pairedChoppas);
        addModel(model);
    }

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *IronskullsBoyz::Create(const ParameterList &parameters)
{
    auto unit = new IronskullsBoyz();

    auto warclan = (Warclan)GetEnumParam("Warclan", parameters, Ironjawz::Ironsunz);
    unit->setWarclan(warclan);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void IronskullsBoyz::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            IronskullsBoyz::Create,
            Ironjawz::ValueToString,
            Ironjawz::EnumStringToInt,
            IronskullsBoyz::ComputePoints,
            {
                {ParamType::Enum, "Warclan", Ironjawz::Ironsunz, Ironjawz::Ironsunz, Ironjawz::DaChoppas, 1},
            },
            DESTRUCTION,
            { IRONJAWZ }
        };
        s_registered = UnitFactory::Register("Ironskull's Boyz", factoryMethod);
    }
}

Wounds IronskullsBoyz::applyWoundSave(const Wounds &wounds)
{
    // Dead 'Ard
    Dice::RollResult result;
    Dice::rollD6(wounds.normal, result);
    Wounds modWounds = wounds;
    modWounds.normal -= result.numUnmodified6s();
    return modWounds;
}

int IronskullsBoyz::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    // Paired Choppas
    if (weapon->name() == m_pairedChoppas.name())
    {
        return 1;
    }
    return Unit::toHitModifier(weapon, target);
}

int IronskullsBoyz::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}


} // namespace Ironjawz
