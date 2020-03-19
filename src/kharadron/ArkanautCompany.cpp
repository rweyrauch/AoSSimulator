/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/ArkanautCompany.h>
#include <UnitFactory.h>
#include <Board.h>

namespace KharadronOverlords
{

bool ArkanautCompany::s_registered = false;

Unit *ArkanautCompany::Create(const ParameterList &parameters)
{
    auto unit = new ArkanautCompany();
    int numModel = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numVolleyGuns = GetIntParam("Volley Guns", parameters, 1);
    int numSkyhooks = GetIntParam("Light Skyhooks", parameters, 1);
    int numSkypikes = GetIntParam("Skypikes", parameters, 1);
    auto option = (CaptainWeapon)GetEnumParam("Captain Weapon", parameters, AetherflarePistol);

    auto port = (Skyport)GetEnumParam("Skyport", parameters, KharadronBase::None);
    unit->setSkyport(port);

    bool ok = unit->configure(numModel, numVolleyGuns, numSkyhooks, numSkypikes, option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string ArkanautCompany::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Captain Weapon")
    {
        if (parameter.intValue == AetherflarePistol) return "Aetherflare Pistol";
        else if (parameter.intValue == PrivateerPistol) return "Privateer Pistol";
    }
    return KharadronBase::ValueToString(parameter);
}

int ArkanautCompany::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Privateer Pistol") return PrivateerPistol;
    else if (enumString == "Aetherflare Pistol") return AetherflarePistol;
    return KharadronBase::EnumStringToInt(enumString);
}

void ArkanautCompany::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            ArkanautCompany::Create,
            ArkanautCompany::ValueToString,
            ArkanautCompany::EnumStringToInt,
            ArkanautCompany::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", ArkanautCompany::MIN_UNIT_SIZE, ArkanautCompany::MIN_UNIT_SIZE,
                    ArkanautCompany::MAX_UNIT_SIZE, ArkanautCompany::MIN_UNIT_SIZE
                },
                { ParamType::Integer, "Volley Guns", 1, 0, ArkanautCompany::MAX_UNIT_SIZE/ArkanautCompany::MIN_UNIT_SIZE, 1},
                { ParamType::Integer, "Light Skyhooks", 1, 0, ArkanautCompany::MAX_UNIT_SIZE/ArkanautCompany::MIN_UNIT_SIZE, 1},
                { ParamType::Integer, "Skypikes", 1, 0, ArkanautCompany::MAX_UNIT_SIZE/ArkanautCompany::MIN_UNIT_SIZE, 1},
                {
                    ParamType::Enum, "Captain Weapon", ArkanautCompany::AetherflarePistol, ArkanautCompany::AetherflarePistol,
                    ArkanautCompany::PrivateerPistol, 1
                },
                {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
            },
            ORDER,
            { KHARADRON_OVERLORDS }
        };
        s_registered = UnitFactory::Register("Arkanaut Company", factoryMethod);
    }
}

ArkanautCompany::ArkanautCompany() :
    KharadronBase("Arkanaut Company", 4, WOUNDS, 6, 4, false),
    m_privateerPistol(Weapon::Type::Missile, "Privateer Pistol", 9, 2, 4, 4, 0, 1),
    m_volleyGun(Weapon::Type::Missile, "Aethermatic Volley Gun", 12, 6, 5, 4, -1, 1),
    m_skyhook(Weapon::Type::Missile, "Light Skyhook", 18, 1, 4, 3, -2, RAND_D3),
    m_aetherflarePistol(Weapon::Type::Missile, "Aetherflare Pistol", 9, 2, 4, 3, 0, 1),
    m_volleyPistol(Weapon::Type::Missile, "Volley Pistol", 9, 3, 4, 4, 0, 1),
    m_cutter(Weapon::Type::Melee, "Arkanaut Cutter", 1, 1, 4, 4, 0, 1),
    m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 5, 0, 1),
    m_skypike(Weapon::Type::Melee, "Skypike", 2, 2, 4, 4, -1, RAND_D3)
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, SKYFARER, MARINE, ARKANAUT_COMPANY};
    m_weapons = {&m_privateerPistol, &m_volleyGun, &m_skyhook, &m_aetherflarePistol, &m_volleyPistol, &m_cutter, &m_gunButt, &m_skypike};
}

bool ArkanautCompany::configure(int numModels, int numVolleyGuns, int numSkyhooks, int numSkypikes, CaptainWeapon option)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }
    const int maxSpecials = numModels / MIN_UNIT_SIZE;
    if (numVolleyGuns > maxSpecials || numSkyhooks > maxSpecials || numSkypikes > maxSpecials)
    {
        return false;
    }

    // Add the Captain
    auto captain = new Model(BASESIZE, WOUNDS);
    if (option == AetherflarePistol)
    {
        captain->addMissileWeapon(&m_aetherflarePistol);
    }
    else if (option == VolleyPistol)
    {
        captain->addMissileWeapon(&m_volleyPistol);
    }
    else if (option == PrivateerPistol)
    {
        captain->addMissileWeapon(&m_privateerPistol);
    }
    captain->addMeleeWeapon(&m_cutter);
    addModel(captain);

    for (auto i = 0; i < numVolleyGuns; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_volleyGun);
        model->addMeleeWeapon(&m_gunButt);
    }
    for (auto i = 0; i < numSkyhooks; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_skyhook);
        model->addMeleeWeapon(&m_gunButt);
    }
    for (auto i = 0; i < numSkypikes; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMeleeWeapon(&m_skypike);
    }

    int currentModelCount = (int) m_models.size();
    for (auto i = currentModelCount; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, WOUNDS);
        model->addMissileWeapon(&m_privateerPistol);
        model->addMeleeWeapon(&m_cutter);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int ArkanautCompany::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

int ArkanautCompany::toHitModifier(const Weapon *weapon, const Unit *target) const
{
    auto mod = Unit::toHitModifier(weapon, target);

    // Glory-seekers
    auto obj = Board::Instance()->getNearestObjective(this);
    if (obj && (distanceTo(obj->m_pos) <= 9.0f))
    {
        mod++;
    }
    return mod;
}

Rerolls ArkanautCompany::battleshockRerolls() const
{
    // Glory-seekers
    auto obj = Board::Instance()->getNearestObjective(this);
    if (obj && (distanceTo(obj->m_pos) <= 9.0f))
    {
        return RerollFailed;
    }

    return Unit::battleshockRerolls();
}

} //KharadronOverlords
