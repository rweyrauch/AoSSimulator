/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Karanak.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Karanak::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    Karanak::ComputePoints,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    { KHORNE }
};

bool Karanak::s_registered = false;

Karanak::Karanak() :
    KhorneBase("Karanak", 8, WOUNDS, 10, 4, false),
    m_goreSlickClaws(Weapon::Type::Melee, "Gore-slick Claws", 1, 4, 3, 4, 0, 1),
    m_savageMaws(Weapon::Type::Melee, "Three Savage Maws", 1, 6, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, FLESH_HOUND, KHORNE, HERO, KARANAK};
    m_weapons = {&m_goreSlickClaws, &m_savageMaws};
}

bool Karanak::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_goreSlickClaws);
    model->addMeleeWeapon(&m_savageMaws);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Karanak::Create(const ParameterList &parameters)
{
    auto unit = new Karanak();

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Karanak::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Karanak", factoryMethod);
    }
}

Rerolls Karanak::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    // Prey of the Blood God
    if (m_pQuarry && (m_pQuarry->name() == target->name()))
    {
        return RerollFailed;
    }
    return KhorneBase::toHitRerolls(weapon, target);
}

Rerolls Karanak::toWoundRerolls(const Weapon *weapon, const Unit *target) const
{
    // Prey of the Blood God
    if (m_pQuarry && (m_pQuarry->name() == target->name()))
    {
        return RerollFailed;
    }
    return KhorneBase::toWoundRerolls(weapon, target);
}

void Karanak::onBeginTurn(int battleRound)
{
    if (battleRound == 1)
    {
        // Select a quarry for Karanak
        // TODO: Select a _good_ unit to be Karanak's quarry
        m_pQuarry = Board::Instance()->getNearestUnit(this, GetEnemyId(m_owningPlayer));
    }

    KhorneBase::onBeginTurn(battleRound);
}

} // namespace Khorne