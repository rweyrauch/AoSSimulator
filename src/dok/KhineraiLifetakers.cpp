/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/KhineraiLifetakers.h>
#include <UnitFactory.h>
#include <iostream>

namespace DaughtersOfKhaine {

static FactoryMethod factoryMethod = {
    KhineraiLifetakers::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Integer, "numModels", {.m_intValue = KhineraiLifetakers::MIN_UNIT_SIZE}, KhineraiLifetakers::MIN_UNIT_SIZE,
         KhineraiLifetakers::MAX_UNIT_SIZE, KhineraiLifetakers::MIN_UNIT_SIZE},
    }
};

Weapon KhineraiLifetakers::s_barbedSickle(Weapon::Type::Melee, "Barbed Sickle", 1, 2, 3, 4, 0, 1);
Weapon KhineraiLifetakers::s_barbedSickleHarridynn(Weapon::Type::Melee, "Barbed Sickle (Harridynn)", 1, 2, 2, 4, 0, 1);

bool KhineraiLifetakers::s_registered = false;

KhineraiLifetakers::KhineraiLifetakers() :
    DaughterOfKhaine("Khinerai Lifetakers", 14, WOUNDS, 7, 6, true)
{
    m_keywords = { ORDER, AELF, DAUGHTERS_OF_KHAINE, KHINERAI_HARPIES, KHINERAI_LIFETAKERS };
}

bool KhineraiLifetakers::configure(int numModels)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_pBarbedSickle = new Weapon(s_barbedSickle);
    m_pBarbedSickleHarridynn = new Weapon(s_barbedSickleHarridynn);

    Model harridynn(BASESIZE, WOUNDS);
    harridynn.addMeleeWeapon(m_pBarbedSickleHarridynn);
    addModel(harridynn);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(m_pBarbedSickle);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_barbedSickle.name() << ": " << s_barbedSickle.strength() << std::endl;
        std::cout << "\t" << s_barbedSickleHarridynn.name() << ": " << s_barbedSickleHarridynn.strength() << std::endl;
    }

    return true;
}

Unit *KhineraiLifetakers::Create(const ParameterList &parameters)
{
    auto unit = new KhineraiLifetakers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);

    bool ok = unit->configure(numModels);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KhineraiLifetakers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Khinerai Lifetakers", factoryMethod);
    }
}

void KhineraiLifetakers::onBeginTurn(int battleRound)
{
    Unit::onBeginTurn(battleRound);

    // reset weapon damage (if previously charged)
    m_pBarbedSickle->setDamage(s_barbedSickle.damage());
    m_pBarbedSickleHarridynn->setDamage(s_barbedSickleHarridynn.damage());
}

void KhineraiLifetakers::onCharged()
{
    Unit::onCharged();

    // Death on the Wind
    m_pBarbedSickle->setDamage(s_barbedSickle.damage()+1);
    m_pBarbedSickleHarridynn->setDamage(s_barbedSickleHarridynn.damage()+1);
}

Wounds KhineraiLifetakers::computeReturnedDamage(const Weapon *weapon,
                                                 const Dice::RollResult &saveRolls) const
{
    // Heartpiercer Shield
    if (!weapon->isMissile())
    {
        // 1 mortal wound for each save of a 6
        Wounds returnedDamage = {0, saveRolls.numUnmodified6s()};
        return returnedDamage;
    }
    return Unit::computeReturnedDamage(weapon, saveRolls);
}

} // namespace DaughtersOfKhaine