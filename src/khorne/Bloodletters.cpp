/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/Bloodletters.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Bloodletters::Create,
    {
        {ParamType::Integer, "numModels", {.m_intValue = Bloodletters::MIN_UNIT_SIZE}, Bloodletters::MIN_UNIT_SIZE,
         Bloodletters::MAX_UNIT_SIZE},
        {ParamType::Boolean, "iconBearer", {.m_boolValue = false}, false, false},
        {ParamType::Boolean, "standardBearer", {.m_boolValue = false}, false, false},
        {ParamType::Boolean, "hornblowers", {.m_boolValue = false}, false, false}
    }
};

bool Bloodletters::s_registered = false;

Weapon Bloodletters::s_hellblade(Weapon::Type::Melee, "Hellblade", 1, 1, 4, 3, -1, 1);

Bloodletters::Bloodletters() :
    Unit("Bloodletters", 5, WOUNDS, 10, 5, false)
{
    m_keywords = {CHAOS, DAEMON, KHORNE, BLOODLETTERS};
}


bool Bloodletters::configure(int numModels, bool iconBearer, bool standardBearer, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_standarBearer = standardBearer;
    m_hornblower = hornblowers;

    m_pHellblade = new Weapon(s_hellblade);
    if (numModels > 20) m_pHellblade->setAttacks(2);

    m_pHellbladeReaper = new Weapon(s_hellblade);
    m_pHellbladeReaper->setAttacks(2);
    if (numModels > 20) m_pHellbladeReaper->setAttacks(3);

    // Add the Hellreaper
    Model reaperModel(BASESIZE, WOUNDS);
    reaperModel.addMeleeWeapon(m_pHellbladeReaper);
    addModel(reaperModel);

    int currentModelCount = (int)m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMeleeWeapon(m_pHellblade);
        addModel(model);
    }

    return true;
}

int Bloodletters::generateMortalWounds(const Weapon *weapon, const Unit *unit, const Hits &hits) const
{
    // Decapitating Blow
    if (hits.rolls.numUnmodified6s() > 0)
    {
        return 1;
    }
    return Unit::generateMortalWounds(weapon, unit, hits);
}

Unit *Bloodletters::Create(const ParameterList &parameters)
{
    auto unit = new Bloodletters();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("iconBearer", parameters, false);
    bool standardBearer = GetBoolParam("standardBearer", parameters, false);
    bool hornblowers = GetBoolParam("hornblowers", parameters, false);

    bool ok = unit->configure(numModels, iconBearer, standardBearer, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bloodletters::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodletters", factoryMethod);
    }
}

} // namespace Khorne