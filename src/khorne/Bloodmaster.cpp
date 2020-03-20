/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodmaster.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne
{
static const int BASESIZE = 40;
static const int WOUNDS = 5;
static const int POINTS_PER_UNIT = 80;

bool Bloodmaster::s_registered = false;

Bloodmaster::Bloodmaster() :
    KhorneBase("Bloodmaster", 5, WOUNDS, 10, 4, false),
    m_bladeOfBlood(Weapon::Type::Melee, "Blade of Blood", 1, 4, 3, 3, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, HERO, HERALD_OF_KHORNE, BLOODMASTER};
    m_weapons = {&m_bladeOfBlood};
}

bool Bloodmaster::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMeleeWeapon(&m_bladeOfBlood);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

Unit *Bloodmaster::Create(const ParameterList &parameters)
{
    auto unit = new Bloodmaster();

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

void Bloodmaster::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            Bloodmaster::Create,
            KhorneBase::ValueToString,
            KhorneBase::EnumStringToInt,
            Bloodmaster::ComputePoints,
            {
                {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
            },
            CHAOS,
            { KHORNE }
        };
        s_registered = UnitFactory::Register("Bloodmaster", factoryMethod);
    }
}

Wounds Bloodmaster::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Decapitating Blow
    if (hitRoll == 6)
    {
        return {weapon->damage(), 1};
    }
    return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int Bloodmaster::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}


} // namespace Khorne