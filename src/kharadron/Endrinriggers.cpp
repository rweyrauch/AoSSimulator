/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/Endrinriggers.h>
#include <UnitFactory.h>

namespace KharadronOverlords
{
static FactoryMethod factoryMethod = {
    Endrinriggers::Create,
    Endrinriggers::ValueToString,
    Endrinriggers::EnumStringToInt,
    {
        {
            ParamType::Integer, "Models", Endrinriggers::MIN_UNIT_SIZE, Endrinriggers::MIN_UNIT_SIZE,
            Endrinriggers::MAX_UNIT_SIZE, Endrinriggers::MIN_UNIT_SIZE
        },
        { ParamType::Integer, "Volley Guns", 1, 0, Endrinriggers::MAX_UNIT_SIZE/Endrinriggers::MIN_UNIT_SIZE, 1},
        { ParamType::Integer, "Skyhooks", 0, 0, Endrinriggers::MAX_UNIT_SIZE/Endrinriggers::MIN_UNIT_SIZE, 1},
        { ParamType::Integer, "Grapnel Launchers", 0, 0, Endrinriggers::MAX_UNIT_SIZE/Endrinriggers::MIN_UNIT_SIZE, 1},
        { ParamType::Integer, "Drill Launchers", 0, 0, Endrinriggers::MAX_UNIT_SIZE/Endrinriggers::MIN_UNIT_SIZE, 1},
        { ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
    },
    ORDER,
    KHARADRON_OVERLORDS
};

bool Endrinriggers::s_registered = false;

Unit *Endrinriggers::Create(const ParameterList &parameters)
{
    auto unit = new Endrinriggers();
    int numModel = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numVolleyGuns = GetIntParam("Volley Guns", parameters, 1);
    int numDrills = GetIntParam("Drill Launchers", parameters, 0);
    int numGrapnels = GetIntParam("Grapnel Launchers", parameters, 1);
    int numSkyhooks = GetIntParam("Skyhooks", parameters, 0);

    bool ok = unit->configure(numModel, numVolleyGuns, numDrills, numGrapnels, numSkyhooks);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string Endrinriggers::ValueToString(const Parameter &parameter)
{
    return KharadronBase::ValueToString(parameter);
}

int Endrinriggers::EnumStringToInt(const std::string &enumString)
{
    return KharadronBase::EnumStringToInt(enumString);
}

void Endrinriggers::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Endrinriggers", factoryMethod);
    }
}

Endrinriggers::Endrinriggers() :
    KharadronBase("Endrinriggers", 12, WOUNDS, 7, 4, true),
    m_volleyGun(Weapon::Type::Missile, "Aeathermatic Volley Gun", 24, 6, 4, 4, -1, 1),
    m_skyhook(Weapon::Type::Missile, "Grapnel Launcher or Skyhook", 24, 1, 4, 3, -2, 3),
    m_drillLauncher(Weapon::Type::Missile, "Drill Launcher", 24, 1, 4, 3, -3, RAND_D3),
    m_rivetGun(Weapon::Type::Missile, "Rapid-fire Rivet Gun", 12, 3, 3, 4, -1, 1),
    m_saw(Weapon::Type::Melee, "Aethermatic Saw", 1, 1, 3, 2, -2, RAND_D3),
    m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 5, 0, 1),
    m_sawMaster(Weapon::Type::Melee, "Aethermatic Saw", 1, 2, 3, 2, -2, RAND_D3),
    m_gunButtMaster(Weapon::Type::Melee, "Gun Butt", 1, 2, 4, 5, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, SKYFARERS, ENDRINRIGGERS};
    m_weapons = {&m_volleyGun, &m_skyhook, &m_drillLauncher, &m_rivetGun, &m_saw,
        &m_gunButt, &m_sawMaster, &m_gunButtMaster};
}

bool Endrinriggers::configure(int numModels, int numVolleyGuns, int numDrills, int numGrapnels, int numSkyhooks)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    const int maxSpecials = numModels / MIN_UNIT_SIZE;
    if (numVolleyGuns > maxSpecials || (numDrills + numSkyhooks + numGrapnels) > maxSpecials)
    {
        return false;
    }

    auto master = new Model(BASESIZE, WOUNDS);
    if (numVolleyGuns > 0)
    {
        master->addMissileWeapon(&m_volleyGun);
        master->addMeleeWeapon(&m_gunButtMaster);
        numVolleyGuns--;
    }
    else if (numDrills > 0)
    {
        master->addMissileWeapon(&m_drillLauncher);
        master->addMeleeWeapon(&m_gunButtMaster);
        numDrills--;
    }
    else if (numSkyhooks > 0)
    {
        master->addMissileWeapon(&m_skyhook);
        master->addMeleeWeapon(&m_gunButtMaster);
        numSkyhooks--;
    }
    else if (numGrapnels > 0)
    {
        master->addMissileWeapon(&m_skyhook);
        master->addMeleeWeapon(&m_gunButtMaster);
        numGrapnels--;
    }
    else
    {
        master->addMissileWeapon(&m_rivetGun);
        master->addMeleeWeapon(&m_sawMaster);
    }
    addModel(master);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        if (numVolleyGuns > 0)
        {
            model->addMissileWeapon(&m_volleyGun);
            model->addMeleeWeapon(&m_gunButt);
            numVolleyGuns--;
        }
        else if (numDrills > 0)
        {
            model->addMissileWeapon(&m_drillLauncher);
            model->addMeleeWeapon(&m_gunButt);
            numDrills--;
        }
        else if (numSkyhooks > 0)
        {
            model->addMissileWeapon(&m_skyhook);
            model->addMeleeWeapon(&m_gunButt);
            numSkyhooks--;
        }
        else if (numGrapnels > 0)
        {
            model->addMissileWeapon(&m_skyhook);
            model->addMeleeWeapon(&m_gunButt);
            numGrapnels--;
        }
        else
        {
            model->addMissileWeapon(&m_rivetGun);
            model->addMeleeWeapon(&m_saw);
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

Wounds Endrinriggers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Drill Launcher
    if ((hitRoll == 6) && (weapon->name() == m_drillLauncher.name()))
    {
        return {0, 3};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

int Endrinriggers::chargeModifier() const
{
    auto mod = Unit::chargeModifier();

    // Skyhook
    for (const auto& m : m_models)
    {
        if (!m->slain() && !m->fled() && m->hasWeapon(m_skyhook.name()))
        {
            mod++;
            break;
        }
    }

    return mod;
}

} //KharadronOverlords
