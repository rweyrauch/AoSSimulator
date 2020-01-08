/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/GrundstokGunhauler.h>
#include <UnitFactory.h>

namespace KharadronOverlords
{
static FactoryMethod factoryMethod = {
    GrundstokGunhauler::Create,
    GrundstokGunhauler::ValueToString,
    GrundstokGunhauler::EnumStringToInt,
    {
        {
            ParamType::Enum, "Weapon", GrundstokGunhauler::SkyCannon, GrundstokGunhauler::SkyCannon,
            GrundstokGunhauler::DrillCannon, 1
        },
        {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
    },
    ORDER,
    KHARADRON_OVERLORDS
};

bool GrundstokGunhauler::s_registered = false;

Unit *GrundstokGunhauler::Create(const ParameterList &parameters)
{
    auto unit = new GrundstokGunhauler();
    auto option = (WeaponOption)GetEnumParam("Weapon", parameters, SkyCannon);

    bool ok = unit->configure(option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string GrundstokGunhauler::ValueToString(const Parameter &parameter)
{
    if (parameter.m_name == "Weapon")
    {
        if (parameter.m_intValue == SkyCannon) return "Sky Cannon";
        else if (parameter.m_intValue == DrillCannon) return "Drill Cannon";
    }
    return KharadronBase::ValueToString(parameter);
}

int GrundstokGunhauler::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Sky Cannon") return SkyCannon;
    else if (enumString == "Drill Cannon") return DrillCannon;
    return KharadronBase::EnumStringToInt(enumString);
}

void GrundstokGunhauler::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Grundstok Gunhauler", factoryMethod);
    }
}

GrundstokGunhauler::GrundstokGunhauler() :
    KharadronBase("Grundstok Gunhauler", 12, WOUNDS, 7, 4, true),
    m_cannonShrapnel(),
    m_cannonShell(),
    m_drillCannon(),
    m_carbines(),
    m_boardingWeapons()
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, WAR_MACHINE, SKYVESSEL, GRUNDSTOK_GUNHAULER};
    m_weapons = {&m_cannonShrapnel,
                 &m_cannonShell,
                 &m_drillCannon,
                 &m_carbines,
                 &m_boardingWeapons};
}

bool GrundstokGunhauler::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, WOUNDS);
    if (option == SkyCannon)
    {
        model->addMissileWeapon(&m_cannonShrapnel);
        model->addMissileWeapon(&m_cannonShell);
        m_cannonShell.activate(false);
    }
    else if (option == DrillCannon)
    {
        model->addMissileWeapon(&m_drillCannon);
    }
    model->addMissileWeapon(&m_carbines);
    model->addMeleeWeapon(&m_boardingWeapons);
    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

} //KharadronOverlords
