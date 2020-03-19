/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/KnightOfShroudsOnSteed.h"

namespace Nighthaunt
{

bool KnightOfShroudsOnEtherealSteed::s_registered = false;

Unit *KnightOfShroudsOnEtherealSteed::Create(const ParameterList &parameters)
{
    auto unit = new KnightOfShroudsOnEtherealSteed();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KnightOfShroudsOnEtherealSteed::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            KnightOfShroudsOnEtherealSteed::Create,
            nullptr,
            nullptr,
            KnightOfShroudsOnEtherealSteed::ComputePoints,
            {
            },
            DEATH,
            { NIGHTHAUNT }
        };
        s_registered = UnitFactory::Register("Knight of Shrouds on Ethereal Steed", factoryMethod);
    }
}

KnightOfShroudsOnEtherealSteed::KnightOfShroudsOnEtherealSteed() :
    Nighthaunt("Knight of Shrouds on Ethereal Steed", 12, WOUNDS, 10, 4, true),
    m_sword(Weapon::Type::Melee, "Sword of Stolen Hours", 1, 4, 3, 3, -1, 2),
    m_hoovesAndTeeth(Weapon::Type::Melee, "Ghostly Hooves and Teeth", 1, 2, 4, 5, 0, 1)
{
    m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, KNIGHT_OF_SHROUDS};
    m_weapons = {&m_sword, &m_hoovesAndTeeth};
}

bool KnightOfShroudsOnEtherealSteed::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_sword);
    model->addMeleeWeapon(&m_hoovesAndTeeth);
    addModel(model);

    m_points = ComputePoints(1);

    return true;
}

} // namespace Nighthaunt
