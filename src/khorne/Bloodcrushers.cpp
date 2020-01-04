/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodcrushers.h>
#include <UnitFactory.h>

namespace Khorne
{
static FactoryMethod factoryMethod = {
    Bloodcrushers::Create,
    KhorneBase::ValueToString,
    KhorneBase::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Bloodcrushers::MIN_UNIT_SIZE, Bloodcrushers::MIN_UNIT_SIZE,
            Bloodcrushers::MAX_UNIT_SIZE, Bloodcrushers::MIN_UNIT_SIZE
        },
        {ParamType::Boolean, "Icon Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Boolean, "Hornblowers", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
        {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None, KhorneBase::SkullfiendTribe, 1}
    },
    CHAOS,
    KHORNE
};

bool Bloodcrushers::s_registered = false;

Bloodcrushers::Bloodcrushers() :
    KhorneBase("Bloodcrushers", 8, WOUNDS, 10, 4, false),
    m_hellblade(Weapon::Type::Melee, "Hellblade", 1, 1, 4, 3, -1, 1),
    m_hellbladeHunter(Weapon::Type::Melee, "Hellblade", 1, 2, 4, 3, -1, 1),
    m_brazenHooves(Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1)
{
    m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, BLOODCRUSHERS};
}

bool Bloodcrushers::configure(int numModels, bool iconBearer, bool hornblowers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_hornblower = hornblowers;

    // Add the Bloodhunter
    auto hunter = new Model(BASESIZE, WOUNDS);
    hunter->addMeleeWeapon(&m_hellbladeHunter);
    hunter->addMeleeWeapon(&m_brazenHooves);
    addModel(hunter);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_hellblade);
        model->addMeleeWeapon(&m_brazenHooves);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Bloodcrushers::visitWeapons(std::function<void(const Weapon &)> &visitor)
{
    visitor(m_hellblade);
    visitor(m_hellbladeHunter);
    visitor(m_brazenHooves);
}

Unit *Bloodcrushers::Create(const ParameterList &parameters)
{
    auto unit = new Bloodcrushers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
    bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

    auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
    unit->setSlaughterHost(host);

    bool ok = unit->configure(numModels, iconBearer, hornblowers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Bloodcrushers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Bloodcrushers", factoryMethod);
    }
}

void Bloodcrushers::onCharged()
{
    // Murderous Charge
    if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0f))
    {
        Dice dice;
        Dice::RollResult rolls;
        dice.rollD6(remainingModels(), rolls);
        Wounds wounds = {0, 0};
        if (remainingModels() >= 6)
        {
            for (int i = 0; i < rolls.rollsGE(2); i++)
            {
                wounds.mortal += dice.rollD3();
            }
        }
        else
        {
            wounds.mortal = rolls.rollsGE(2);
        }

        SimLog(Verbosity::Narrative, "%s Murderous Charge inflicted %d mortal wounds on %s\n",
               name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

        m_meleeTarget->applyDamage(wounds);
    }
    KhorneBase::onCharged();
}

Wounds Bloodcrushers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Decapitating Blow
    if (hitRoll == 6)
    {
        return {weapon->damage(), 1};
    }
    return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace Khorne
