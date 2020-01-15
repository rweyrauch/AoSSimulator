/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "idonethdeepkin/EidolonSea.h"

namespace IdonethDeepkin
{
static FactoryMethod factoryMethod = {
    EidolonOfMathlannAspectOfTheSea::Create,
    nullptr,
    nullptr,
    EidolonOfMathlannAspectOfTheSea::ComputePoints,
    {
    },
    ORDER,
    { IDONETH_DEEPKIN }
};

bool EidolonOfMathlannAspectOfTheSea::s_registered = false;

Unit *EidolonOfMathlannAspectOfTheSea::Create(const ParameterList &parameters)
{
    auto unit = new EidolonOfMathlannAspectOfTheSea();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void EidolonOfMathlannAspectOfTheSea::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Eidolon of Mathlann Aspect of the Sea", factoryMethod);
    }
}

EidolonOfMathlannAspectOfTheSea::EidolonOfMathlannAspectOfTheSea() :
    Unit("Eidolon of Mathlann Aspect of the Sea", 10, WOUNDS, 10, 3, true),
    m_abyssalEnergy(Weapon::Type::Missile, "Blasts of Abyssal Energy", 15, RAND_D3, 3, 3, -2, 2),
    m_trident(Weapon::Type::Melee, "Psi-trident", 2, 3, 3, 3, -2, 2),
    m_sceptre(Weapon::Type::Melee, "Deep-sea Sceptre", 1, 3, 3, 3, -1, 1),
    m_stormshoal(Weapon::Type::Melee, "Stormshoal", 3, RAND_2D6, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, EIDOLON, HERO, WIZARD, ASPECT_OF_THE_SEA};
    m_weapons = {&m_abyssalEnergy, &m_trident, &m_sceptre, &m_stormshoal};
}

bool EidolonOfMathlannAspectOfTheSea::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_abyssalEnergy);
    model->addMeleeWeapon(&m_trident);
    model->addMeleeWeapon(&m_sceptre);
    model->addMeleeWeapon(&m_stormshoal);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} // IdonethDeepkin

