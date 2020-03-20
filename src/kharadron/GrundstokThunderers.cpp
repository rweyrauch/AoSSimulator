/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/GrundstokThunderers.h>
#include <UnitFactory.h>

namespace KharadronOverlords
{

bool GrundstokThunderers::s_registered = false;

Unit *GrundstokThunderers::Create(const ParameterList &parameters)
{
    auto unit = new GrundstokThunderers();
    int numModel = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
    int numMortars = GetIntParam("Grundstok Mortars", parameters, 1);
    int numCannons = GetIntParam("Aethercannons", parameters, 1);
    int numFumigators = GetIntParam("Aetheric Fumigators", parameters, 1);
    int numDecksweepers = GetIntParam("Desksweepers", parameters, 1);
    int numHonourBearers = GetIntParam("Honour Bearers", parameters, 1);

    auto port = (Skyport)GetEnumParam("Skyport", parameters, KharadronBase::None);
    unit->setSkyport(port);

    bool ok = unit->configure(numModel, numMortars, numCannons, numFumigators, numDecksweepers, numHonourBearers);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string GrundstokThunderers::ValueToString(const Parameter &parameter)
{
    return KharadronBase::ValueToString(parameter);
}

int GrundstokThunderers::EnumStringToInt(const std::string &enumString)
{
    return KharadronBase::EnumStringToInt(enumString);
}

void GrundstokThunderers::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            GrundstokThunderers::Create,
            GrundstokThunderers::ValueToString,
            GrundstokThunderers::EnumStringToInt,
            GrundstokThunderers::ComputePoints,
            {
                {
                    ParamType::Integer, "Models", GrundstokThunderers::MIN_UNIT_SIZE, GrundstokThunderers::MIN_UNIT_SIZE,
                    GrundstokThunderers::MAX_UNIT_SIZE, GrundstokThunderers::MIN_UNIT_SIZE
                },
                { ParamType::Integer, "Grundstok Mortars", 1, 0, GrundstokThunderers::MAX_UNIT_SIZE/GrundstokThunderers::MIN_UNIT_SIZE, 1},
                { ParamType::Integer, "Aethercannons", 1, 0, GrundstokThunderers::MAX_UNIT_SIZE/GrundstokThunderers::MIN_UNIT_SIZE, 1},
                { ParamType::Integer, "Aetheric Fumigator", 1, 0, GrundstokThunderers::MAX_UNIT_SIZE/GrundstokThunderers::MIN_UNIT_SIZE, 1},
                { ParamType::Integer, "Desksweepers", 1, 0, GrundstokThunderers::MAX_UNIT_SIZE/GrundstokThunderers::MIN_UNIT_SIZE, 1},
                { ParamType::Integer, "Honour Bearers", 1, 0, GrundstokThunderers::MAX_UNIT_SIZE/GrundstokThunderers::MIN_UNIT_SIZE, 1},
                { ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
            },
            ORDER,
            { KHARADRON_OVERLORDS }
        };
        s_registered = UnitFactory::Register("Endrinriggers", factoryMethod);
    }
}

GrundstokThunderers::GrundstokThunderers() :
    KharadronBase("Grundstok Thunderers", 4, WOUNDS, 7, 4, false),
    m_rifle(Weapon::Type::Missile, "Aethershot Rifle", 18, 2, 3, 4, -1, 1),
    m_doubleBarrelledRifle(Weapon::Type::Missile, "Double-barrelled Aethershot Rifle", 18, 4, 3, 4, -1, 1),
    m_fumigator(Weapon::Type::Missile, "Aetheric Fumigator", 9, 3, 3, 3, -1, 1),
    m_decksweeper(Weapon::Type::Missile, "Desksweeper", 12, 4, 4, 4, -1, 1),
    m_aethercannon(Weapon::Type::Missile, "Aethercannon", 12, 1, 4, 2, -2, RAND_D3),
    m_mortar(Weapon::Type::Missile, "Grundstok Mortar", 12, 1, 4, 3, 0, RAND_D3),
    m_drillbill(Weapon::Type::Melee, "Drillbill", 3, RAND_D3, 4, 4, -1, 1),
    m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, SKYFARER, MARINE, GRUNDSTOK_THUNDERERS};
    m_weapons = {&m_rifle,
        &m_doubleBarrelledRifle,
        &m_fumigator,
        &m_decksweeper,
        &m_aethercannon,
        &m_mortar,
        &m_drillbill,
        &m_gunButt};
}

bool GrundstokThunderers::configure(int numModels, int numMortars, int numCannons, int numFumigators, int numDecksweeper, int numHonourBearers)
{
    if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE)
    {
        return false;
    }

    const int maxSpecials = numModels / MIN_UNIT_SIZE;
    if (numMortars > maxSpecials || numCannons > maxSpecials || numFumigators > maxSpecials || numDecksweeper > maxSpecials || numHonourBearers > maxSpecials)
    {
        return false;
    }

    auto sergeant = new Model(BASESIZE, wounds());
    sergeant->addMissileWeapon(&m_doubleBarrelledRifle);
    sergeant->addMeleeWeapon(&m_drillbill);
    sergeant->addMeleeWeapon(&m_gunButt);
    addModel(sergeant);

    for (auto i = 1; i < numModels; i++)
    {
        auto model = new Model(BASESIZE, wounds());
        if (numMortars > 0)
        {
            model->addMissileWeapon(&m_mortar);
            numMortars--;
        }
        else if (numCannons > 0)
        {
            model->addMissileWeapon(&m_aethercannon);
            numCannons--;
        }
        else if (numDecksweeper > 0)
        {
            model->addMissileWeapon(&m_decksweeper);
            numDecksweeper--;
        }
        else if (numFumigators > 0)
        {
            model->addMissileWeapon(&m_fumigator);
            numFumigators--;
        }
        else
        {
            model->addMissileWeapon(&m_rifle);
        }
        if (numHonourBearers > 0)
        {
            model->setName("Honour Bearer");
            numHonourBearers--;
        }
        model->addMeleeWeapon(&m_gunButt);
        addModel(model);
    }

    m_points = ComputePoints(numModels);

    return true;
}

int GrundstokThunderers::ComputePoints(int numModels)
{
    auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
    if (numModels == MAX_UNIT_SIZE)
    {
        points = POINTS_MAX_UNIT_SIZE;
    }
    return points;
}

} //KharadronOverlords
