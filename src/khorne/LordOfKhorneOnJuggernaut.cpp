/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/LordOfKhorneOnJuggernaut.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    LordOfKhorneOnJuggernaut::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    {
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool LordOfKhorneOnJuggernaut::s_registered = false;

LordOfKhorneOnJuggernaut::LordOfKhorneOnJuggernaut() :
    KhorneBase("Lord of Khorne on Juggernaut", 5, WOUNDS, 9, 3, false),
    m_wrathforgedAxe(Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3),
    m_brazenHooves(Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, LORD_OF_KHORNE_ON_JUGGERNAUT};
}

bool LordOfKhorneOnJuggernaut::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_wrathforgedAxe);
    model.addMeleeWeapon(&m_brazenHooves);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void LordOfKhorneOnJuggernaut::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_wrathforgedAxe);
    visitor(m_brazenHooves);
}

Unit *LordOfKhorneOnJuggernaut::Create(const ParameterList &parameters)
{
    auto unit = new LordOfKhorneOnJuggernaut();

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

void LordOfKhorneOnJuggernaut::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Lord of Khorne on Juggernaut", factoryMethod);
    }
}

void LordOfKhorneOnJuggernaut::onCharged()
{
    // Slaughterous Charge
    if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0f))
    {
        Dice dice;
        int roll = dice.rollD6();
        if (roll >= 2)
        {
            Wounds wounds = {0, dice.rollD3()};

            SimLog(Verbosity::Narrative, "%s Murderous Charge inflicted %d mortal wounds on %s\n",
                   name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

            m_meleeTarget->applyDamage(wounds);
        }
    }
    KhorneBase::onCharged();
}

Wounds LordOfKhorneOnJuggernaut::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Daemonic Axe
    if ((hitRoll == 6) && (weapon->name() == m_wrathforgedAxe.name()))
    {
        return {RAND_D3, 0};
    }
    return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}


} // namespace Khorne