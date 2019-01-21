/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/Bloodreavers.h>
#include <UnitFactory.h>
#include <iostream>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Bloodreavers::Create,
    {
        {ParamType::Integer, "numModels", {.m_intValue = Bloodreavers::MIN_UNIT_SIZE}, Bloodreavers::MIN_UNIT_SIZE,
         Bloodreavers::MAX_UNIT_SIZE},
        {ParamType::Integer, "weapons", {.m_intValue = Bloodreavers::ReaverBlades}, Bloodreavers::ReaverBlades,
         Bloodreavers::MeatripperAxe},
        {ParamType::Boolean, "iconBearer", {.m_boolValue = true}, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = true}, false, false}
    }
};

bool Bloodreavers::s_registered = false;

Weapon Bloodreavers::s_reaverBlades(Weapon::Type::Melee, "Reaver Blades", 1, 1, 4, 4, 0, 1);
Weapon Bloodreavers::s_reaverBladesChieftain(Weapon::Type::Melee, "Reaver Blades (Chief)", 1, 2, 4, 4, 0, 1);
Weapon Bloodreavers::s_meatripperAxe(Weapon::Type::Melee, "Meatripper Axe", 1, 1, 4, 4, -1, 1);
Weapon Bloodreavers::s_meatripperAxeChieftain(Weapon::Type::Melee, "Meatripper Axe (Chief)", 1, 2, 4, 4, -1, 1);

Bloodreavers::Bloodreavers() :
    Unit("Bloodreavers", 6, WOUNDS, 5, 6, false)
{
    m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, BLOODREAVERS};
}


bool Bloodreavers::configure(int numModels, Bloodreavers::WeaponOption weapons, bool iconBearer, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weaponOption = weapons;
    m_iconBearer = iconBearer;
    m_hornblower = hornblowers;

    // Add the Chieftain
    Model chieftainModel(BASESIZE, WOUNDS);
    if (m_weaponOption == ReaverBlades)
    {
        chieftainModel.addMeleeWeapon(&s_reaverBladesChieftain);
    }
    else if (m_weaponOption == MeatripperAxe)
    {
        chieftainModel.addMeleeWeapon(&s_meatripperAxeChieftain);
    }
    addModel(chieftainModel);

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        if (m_weaponOption == ReaverBlades)
            model.addMeleeWeapon(&s_reaverBlades);
        else if (m_weaponOption == MeatripperAxe)
            model.addMeleeWeapon(&s_meatripperAxe);
        addModel(model);
    }

    if (m_verbose)
    {
        std::cout << name() << " Weapon Strengths:" << std::endl;
        std::cout << "\t" << s_reaverBlades.name() << ": " << s_reaverBlades.strength() << std::endl;
        std::cout << "\t" << s_reaverBladesChieftain.name() << ": " << s_reaverBladesChieftain.strength() << std::endl;
        std::cout << "\t" << s_meatripperAxe.name() << ": " << s_meatripperAxe.strength() << std::endl;
        std::cout << "\t" << s_meatripperAxeChieftain.name() << ": " << s_meatripperAxeChieftain.strength() << std::endl;
    }

    return true;
}

Rerolls Bloodreavers::toHitRerolls(const Weapon* weapon, const Unit* unit) const
{
    // Reaver Blades
    if (m_weaponOption == ReaverBlades)
        return RerollOnes;

    return Unit::toHitRerolls(weapon, unit);
}

int Bloodreavers::battlshockModifier() const
{
    int modifier = Unit::battlshockModifier();

    // Icon Bearer
    if (m_iconBearer)
        modifier++;

    return modifier;
}

Unit *Bloodreavers::Create(const ParameterList &parameters)
{
    auto unit = new Bloodreavers();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOption weapons = (WeaponOption)GetIntParam("weapons", parameters, ReaverBlades);
    bool iconBearer = GetBoolParam("iconBearer", parameters, false);
    bool hornblowers = GetBoolParam("hornblowers", parameters, false);

    bool ok = unit->configure(numModels, weapons, iconBearer, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bloodreavers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodreavers", factoryMethod);
    }
}

} // namespace Khorne