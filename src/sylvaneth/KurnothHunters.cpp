/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/KurnothHunters.h>
#include <UnitFactory.h>
#include <iostream>

namespace Sylvaneth
{
static FactoryMethod factoryMethod = {
    KurnothHunters::Create,
    KurnothHunters::ValueToString,
    {
        {ParamType::Integer, "numModels", {.m_intValue = KurnothHunters::MIN_UNIT_SIZE}, KurnothHunters::MIN_UNIT_SIZE,
         KurnothHunters::MAX_UNIT_SIZE, KurnothHunters::MIN_UNIT_SIZE},
        {ParamType::Integer, "weapons", {.m_intValue = KurnothHunters::Greatswords}, KurnothHunters::Greatswords,
         KurnothHunters::Greatbows, 1}
    }
};

bool KurnothHunters::s_registered = false;

Weapon KurnothHunters::s_greatbow(Weapon::Type::Missile, "Kurnoth Greatbow", 30, 2, 4, 3, -1, RAND_D3);
Weapon KurnothHunters::s_greatbowHuntmaster(Weapon::Type::Missile, "Kurnoth Greatbow (Huntmaster)", 30, 2, 3, 3, -1, RAND_D3);
Weapon KurnothHunters::s_greatsword(Weapon::Type::Melee, "Kurnoth Greatsword", 1, 4, 3, 3, -1, 2);
Weapon KurnothHunters::s_greatswordHuntmaster(Weapon::Type::Melee, "Kurnoth Greatsword (Huntmaster)", 1, 4, 2, 3, -1, 2);
Weapon KurnothHunters::s_scythe(Weapon::Type::Melee, "Kurnoth Scythe", 2, 3, 3, 3, -2, RAND_D3);
Weapon KurnothHunters::s_scytheHuntmaster(Weapon::Type::Melee, "Kurnoth Scythe (Huntmaster)", 2, 3, 2, 3, -2, RAND_D3);
Weapon KurnothHunters::s_viciousClaws(Weapon::Type::Melee, "Quiverling's Vicious Claws", 1, 3, 4, 4, 0, 1);

KurnothHunters::KurnothHunters() :
    Unit("Kurnoth Hunters", 5, WOUNDS, 7, 4, false)
{
    m_keywords = { ORDER, SYLVANETH, KURNOTH_HUNTERS };
}

bool KurnothHunters::configure(int numModels, WeaponOption weapons)
{
    // validate inputs
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        // Invalid model count.
        return false;
    }

    m_weaponOption = weapons;

    Model huntmaster(BASESIZE, WOUNDS);
    if (m_weaponOption == Greatbows)
    {
        huntmaster.addMissileWeapon(&s_greatbowHuntmaster);
        huntmaster.addMeleeWeapon(&s_viciousClaws);
    }
    else if (m_weaponOption == Greatswords)
    {
        huntmaster.addMeleeWeapon(&s_greatswordHuntmaster);
    }
    else if (m_weaponOption == Scythes)
    {
        huntmaster.addMeleeWeapon(&s_scytheHuntmaster);
    }
    addModel(huntmaster);

    for (int i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == Greatbows)
        {
            model.addMissileWeapon(&s_greatbow);
            model.addMeleeWeapon(&s_viciousClaws);
        }
        else if (m_weaponOption == Greatswords)
        {
            model.addMeleeWeapon(&s_greatsword);
        }
        else if (m_weaponOption == Scythes)
        {
            model.addMeleeWeapon(&s_scythe);
        }
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_greatbow.name() << ": " << s_greatbow.strength() << std::endl;
        std::cout << "\t" << s_greatbowHuntmaster.name() << ": " << s_greatbowHuntmaster.strength() << std::endl;
        std::cout << "\t" << s_greatsword.name() << ": " << s_greatsword.strength() << std::endl;
        std::cout << "\t" << s_greatswordHuntmaster.name() << ": " << s_greatswordHuntmaster.strength() << std::endl;
        std::cout << "\t" << s_scythe.name() << ": " << s_scythe.strength() << std::endl;
        std::cout << "\t" << s_scytheHuntmaster.name() << ": " << s_scytheHuntmaster.strength() << std::endl;
        std::cout << "\t" << s_viciousClaws.name() << ": " << s_viciousClaws.strength() << std::endl;
    }

    return true;
}

Unit *KurnothHunters::Create(const ParameterList &parameters)
{
    auto unit = new KurnothHunters();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption)GetIntParam("weapons", parameters, KurnothHunters::Greatswords);

    bool ok = unit->configure(numModels, weapons);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void KurnothHunters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Kurnoth Hunters", factoryMethod);
    }
}

std::string KurnothHunters::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "weapons")
    {
        if (parameter.m_intValue == Greatswords)
            return "Greatswords";
        else if (parameter.m_intValue == Scythes)
            return "Scythes";
        else if (parameter.m_intValue == Greatbows)
            return "Greatbows";
    }
    return ParameterValueToString(parameter);
}

} // namespace Sylvaneth
