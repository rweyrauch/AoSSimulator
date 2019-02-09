/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Irondrakes.h>
#include <UnitFactory.h>

namespace Dispossessed
{
static FactoryMethod factoryMethod = {
    Irondrakes::Create,
    nullptr,
    nullptr,
    {
        {
            ParamType::Integer, "numModels", {.m_intValue = Irondrakes::MIN_UNIT_SIZE}, Irondrakes::MIN_UNIT_SIZE,
            Irondrakes::MAX_UNIT_SIZE, Irondrakes::MIN_UNIT_SIZE
        },
        {
            ParamType::Integer, "ironbeardWeapons", {.m_intValue = Irondrakes::Drakegun}, Irondrakes::Drakegun,
            Irondrakes::PairedDrakefirePistols, 1
        },
        {ParamType::Boolean, "iconBearer", {.m_boolValue = false}, false, false, false},
        {ParamType::Boolean, "hornblower", {.m_boolValue = false}, false, false, false}
    }
};

bool Irondrakes::s_registered = false;

Irondrakes::Irondrakes() :
    Unit("Irondrakes", 4, WOUNDS, 7, 4, false),
    m_drakegun(Weapon::Type::Missile, "Drakegun", 16, 1, 3, 3, -1, 1),
    m_drakegunWarden(Weapon::Type::Missile, "Drakegun (Ironwarden)", 16, 1, 2, 3, -1, 1),
    m_grudgehammerTorpedo(Weapon::Type::Missile, "Grudgehammer Torpedo", 20, 1, 3, 3, -2, RAND_D3),
    m_drakefirePistol(Weapon::Type::Missile, "Drakefire Pistol", 8, 1, 4, 3, -1, 1),
    m_drakefirePistolMelee(Weapon::Type::Melee, "Drakefire Pistol", 1, 1, 4, 4, 0, 1),
    m_mailedFist(Weapon::Type::Melee, "Mailed Fist", 1, 1, 4, 5, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, DISPOSSESSED, IRONDRAKES};
}

bool Irondrakes::configure(int numModels, WeaponOptions ironWardenWeapons, bool iconBearer, bool hornblower)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_iconBearer = iconBearer;
    m_hornblower = hornblower;

    Model ironwarden(BASESIZE, WOUNDS);
    if (ironWardenWeapons == Drakegun)
    {
        ironwarden.addMissileWeapon(&m_drakegunWarden);
        ironwarden.addMeleeWeapon(&m_mailedFist);
    }
    else if (ironWardenWeapons == DrakefirePistolAndCinderblastBomb)
    {
        ironwarden.addMissileWeapon(&m_drakefirePistol);
        ironwarden.addMeleeWeapon(&m_drakefirePistolMelee);
        m_hasCinderblastBomb = true;
    }
    else if (ironWardenWeapons == PairedDrakefirePistols)
    {
        // two attacks when using dual-pistols
        ironwarden.addMissileWeapon(&m_drakefirePistol);
        ironwarden.addMissileWeapon(&m_drakefirePistol);
        ironwarden.addMeleeWeapon(&m_drakefirePistolMelee);
        ironwarden.addMeleeWeapon(&m_drakefirePistolMelee);
    }
    else if (ironWardenWeapons == GrudgehammerTorpedo)
    {
        ironwarden.addMissileWeapon(&m_grudgehammerTorpedo);
        ironwarden.addMeleeWeapon(&m_mailedFist);
    }
    addModel(ironwarden);

    for (auto i = 1; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_drakegun);
        model.addMeleeWeapon(&m_mailedFist);
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void Irondrakes::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_drakegun);
    visitor(&m_drakegunWarden);
    visitor(&m_grudgehammerTorpedo);
    visitor(&m_drakefirePistol);
    visitor(&m_drakefirePistolMelee);
    visitor(&m_mailedFist);
}

Unit *Irondrakes::Create(const ParameterList &parameters)
{
    auto unit = new Irondrakes();
    int numModels = GetIntParam("numModels", parameters, MIN_UNIT_SIZE);
    WeaponOptions weapon = (WeaponOptions)GetIntParam("ironWardenWeapons", parameters, (int)Drakegun);
    bool iconBearer = GetBoolParam("iconBearer", parameters, false);
    bool hornblower = GetBoolParam("hornblower", parameters, false);

    bool ok = unit->configure(numModels, weapon, iconBearer, hornblower);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Irondrakes::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Irondrakes", factoryMethod);
    }
}

} // namespace Dispossessed
