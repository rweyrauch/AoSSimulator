/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/VulkiteBerzerkers.h>
#include <UnitFactory.h>

namespace Fyreslayers
{
static FactoryMethod factoryMethod = {
    VulkiteBerzerkers::Create,
    VulkiteBerzerkers::ValueToString,
    VulkiteBerzerkers::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", VulkiteBerzerkers::MIN_UNIT_SIZE, VulkiteBerzerkers::MIN_UNIT_SIZE,
            VulkiteBerzerkers::MAX_UNIT_SIZE, VulkiteBerzerkers::MIN_UNIT_SIZE
        },
        {
            ParamType::Enum, "Weapons", VulkiteBerzerkers::HandaxeAndShield, VulkiteBerzerkers::HandaxeAndShield,
            VulkiteBerzerkers::PairedHandaxes, 1
        },
        {ParamType::Boolean, "Horn of Grimnir", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0}
    },
    ORDER,
    FYRESLAYERS
};

bool VulkiteBerzerkers::s_registered = false;

VulkiteBerzerkers::VulkiteBerzerkers() :
    Unit("Vulkite Berzerkers", 4, WOUNDS, 7, 5, false),
    m_handaxe(Weapon::Type::Melee, "Fyresteel Handaxe", 1, 2, 3, 3, 0, 1),
    m_handaxeKarl(Weapon::Type::Melee, "Fyresteel Handaxe (Karl)", 1, 3, 3, 3, 0, 1),
    m_warpick(Weapon::Type::Melee, "Fyresteel War-pick", 1, 2, 3, 4, -1, 1),
    m_warpickKarl(Weapon::Type::Melee, "Fyresteel War-pick (Karl)", 1, 3, 3, 4, -1, 1),
    m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, FYRESLAYERS, VULKITE_BERZERKERS};
}

bool VulkiteBerzerkers::configure(int numModels, WeaponOption weapons, bool hornOfGrimnir)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    m_weapons = weapons;
    m_hornOfGrimnir = hornOfGrimnir;

    Model karl(BASESIZE, WOUNDS);
    karl.addMissileWeapon(&m_throwingAxe);
    if (weapons == HandaxeAndShield || weapons == PairedHandaxes)
    {
        karl.addMeleeWeapon(&m_handaxeKarl);
    }
    else if (weapons == WarpickAndShield)
    {
        karl.addMeleeWeapon(&m_warpickKarl);
    }
    addModel(karl);

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        Model model(BASESIZE, WOUNDS);
        model.addMissileWeapon(&m_throwingAxe);
        if (weapons == HandaxeAndShield || weapons == PairedHandaxes)
        {
            model.addMeleeWeapon(&m_handaxe);
        }
        else if (weapons == WarpickAndShield)
        {
            model.addMeleeWeapon(&m_warpick);
        }
        addModel(model);
    }

    m_points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        m_points = POINTS_MAX_UNIT_SIZE;
    }

    return true;
}

void VulkiteBerzerkers::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_handaxe);
    visitor(&m_handaxeKarl);
    visitor(&m_warpick);
    visitor(&m_warpickKarl);
    visitor(&m_throwingAxe);
}

Unit *VulkiteBerzerkers::Create(const ParameterList &parameters)
{
    auto unit = new VulkiteBerzerkers();
    int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    auto weapons = (WeaponOption)GetEnumParam("Weapons", parameters, HandaxeAndShield);
    auto horn = GetBoolParam("Horn of Grimnir", parameters, false);

    bool ok = unit->configure(numModels, weapons, horn);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void VulkiteBerzerkers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Vulkite Berzerkers", factoryMethod);
    }
}

std::string VulkiteBerzerkers::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapons")
    {
        if (parameter.m_intValue == HandaxeAndShield) { return "Fyresteel Handaxe and Bladed Slingshield"; }
        else if (parameter.m_intValue == WarpickAndShield) { return "Fyresteel War-pick and Bladed Slingshield"; }
        else if (parameter.m_intValue == PairedHandaxes) { return "Paired Fyresteel Handaxes"; }
    }
    return ParameterValueToString(parameter);
}

int VulkiteBerzerkers::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Fyresteel Handaxe and Bladed Slingshield") { return HandaxeAndShield; }
    else if (enumString == "Fyresteel War-pick and Bladed Slingshield") { return WarpickAndShield; }
    else if (enumString == "Paired Fyresteel Handaxes") { return PairedHandaxes; }
    return 0;
}

Rerolls VulkiteBerzerkers::toHitRerolls(const Weapon *weapon, const Unit *target) const
{
    if ((weapon->name() == m_handaxe.name()) && (m_weapons == PairedHandaxes))
    {
        return RerollFailed;
    }
    return Unit::toHitRerolls(weapon, target);
}

} //namespace Fyreslayers