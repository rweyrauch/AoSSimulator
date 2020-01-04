/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/BloodThrone.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    HeraldOfKhorneOnBloodThrone::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool HeraldOfKhorneOnBloodThrone::s_registered = false;

HeraldOfKhorneOnBloodThrone::HeraldOfKhorneOnBloodThrone() :
    KhorneBase("Herald of Khorne on Blood Throne", 8, WOUNDS, 10, 4, false),
    m_bladeOfBlood(Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1),
    m_hellblades(Weapon::Type::Melee, "Hellblades", 1, 2, 4, 3, -1, 1),
    m_gnashingMaw(Weapon::Type::Melee, "Gnashing Maw", 1, 1, 4, 3, -1, RAND_D3)
{
    m_keywords = {CHAOS, DAEMON, KHORNE, BLOODLETTER, HERO, HERALD_OF_KHORNE, HERALD_OF_KHORNE_ON_BLOOD_THRONE};
}

bool HeraldOfKhorneOnBloodThrone::configure()
{
        auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_bladeOfBlood);
    model->addMeleeWeapon(&m_hellblades);
    model->addMeleeWeapon(&m_gnashingMaw);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void HeraldOfKhorneOnBloodThrone::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_bladeOfBlood);
    visitor(m_hellblades);
    visitor(m_gnashingMaw);
}

Unit *HeraldOfKhorneOnBloodThrone::Create(const ParameterList &parameters)
{
    auto unit = new HeraldOfKhorneOnBloodThrone();

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

void HeraldOfKhorneOnBloodThrone::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Herald of Khorne on Blood Throne", factoryMethod);
    }
}

Wounds HeraldOfKhorneOnBloodThrone::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Decapitating Blow
    if (hitRoll == 6)
    {
        return {weapon->damage(), 1};
    }
    return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace Khorne