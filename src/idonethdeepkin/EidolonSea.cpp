/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "idonethdeepkin/EidolonSea.h"

namespace IdonethDeepkin
{

bool EidolonOfMathlannAspectOfTheSea::s_registered = false;

Unit *EidolonOfMathlannAspectOfTheSea::Create(const ParameterList &parameters)
{
    auto unit = new EidolonOfMathlannAspectOfTheSea();

    auto enclave = (Enclave)GetEnumParam("Enclave", parameters, Enclave::None);
    unit->setEnclave(enclave);

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
        static auto factoryMethod = new FactoryMethod{
            Create,
            IdonethDeepkinBase::ValueToString,
            IdonethDeepkinBase::EnumStringToInt,
            ComputePoints,
            {
                {ParamType::Enum, "Enclave", IdonethDeepkinBase::None, IdonethDeepkinBase::None, IdonethDeepkinBase::Briomdar, 1},
            },
            ORDER,
            { IDONETH_DEEPKIN }
        };

        s_registered = UnitFactory::Register("Eidolon of Mathlann Aspect of the Sea", *factoryMethod);
    }
}

EidolonOfMathlannAspectOfTheSea::EidolonOfMathlannAspectOfTheSea() :
    IdonethDeepkinBase("Eidolon of Mathlann Aspect of the Sea", 10, WOUNDS, 10, 3, true),
    m_abyssalEnergy(Weapon::Type::Missile, "Blasts of Abyssal Energy", 15, RAND_D3, 3, 3, -2, 2),
    m_trident(Weapon::Type::Melee, "Psi-trident", 2, 3, 3, 3, -2, 2),
    m_sceptre(Weapon::Type::Melee, "Deep-sea Sceptre", 1, 3, 3, 3, -1, 1),
    m_stormshoal(Weapon::Type::Melee, "Stormshoal", 3, RAND_2D6, 4, 4, 0, 1)
{
    m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, EIDOLON, HERO, WIZARD, ASPECT_OF_THE_SEA};
    m_weapons = {&m_abyssalEnergy, &m_trident, &m_sceptre, &m_stormshoal};

    s_globalBraveryMod.connect(this, &EidolonOfMathlannAspectOfTheSea::tranquilityOfTheAbyss, &m_connection);

    m_totalSpells = 1;
    m_totalUnbinds = 1;
}

EidolonOfMathlannAspectOfTheSea::~EidolonOfMathlannAspectOfTheSea()
{
    m_connection.disconnect();
}

bool EidolonOfMathlannAspectOfTheSea::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMissileWeapon(&m_abyssalEnergy);
    model->addMeleeWeapon(&m_trident);
    model->addMeleeWeapon(&m_sceptre);
    model->addMeleeWeapon(&m_stormshoal);

    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

int EidolonOfMathlannAspectOfTheSea::tranquilityOfTheAbyss(const Unit *target)
{
    // Tranquility of the Abyss
    if (target->hasKeyword(IDONETH_DEEPKIN) && (target->owningPlayer() == owningPlayer()) && (distanceTo(target) <= 9.0f))
    {
        return 3;
    }

    return 0;
}

} // IdonethDeepkin

