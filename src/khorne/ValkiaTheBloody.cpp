/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/ValkiaTheBloody.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    ValkiaTheBloody::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    { KHORNE }
};

bool ValkiaTheBloody::s_registered = false;

ValkiaTheBloody::ValkiaTheBloody() :
    KhorneBase("Valkia the Bloody", 12, WOUNDS, 9, 3, true),
    m_slaupnir(Weapon::Type::Melee, "Slaupnir", 2, 6, 3, 3, -2, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, VALKIA_THE_BLOODY};
    m_weapons = {&m_slaupnir};
}

bool ValkiaTheBloody::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_slaupnir);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *ValkiaTheBloody::Create(const ParameterList &parameters)
{
    auto unit = new ValkiaTheBloody();

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

void ValkiaTheBloody::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Valkia the Bloody", factoryMethod);
    }
}

Wounds ValkiaTheBloody::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // The Spear Slaupnir
    if (m_charged && (weapon->name() == m_slaupnir.name()))
    {
        return {RAND_D3, 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int ValkiaTheBloody::targetWoundModifier(const Weapon *weapon, const Unit *attacker) const
{
    // Daemonshield
    if (!weapon->isMissile())
    {
        return -1;
    }
    return Unit::targetWoundModifier(weapon, attacker);
}

} // namespace Khorne