/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/MortekCrawler.h"

namespace OssiarchBonereapers
{
static const int BASESIZE = 170; // x105 oval
static const int WOUNDS = 12;
static const int POINTS_PER_UNIT = 200;

struct TableEntry
{
    int m_skullDamage;
    int m_cauldron;
    int m_stele;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {5, 0, -3},
        {4, 0, -1},
        {3, -1, 0},
        {2,  -1, 1},
        {1,  -2, 3}
    };

bool MortekCrawler::s_registered = false;

Unit *MortekCrawler::Create(const ParameterList &parameters)
{
    auto unit = new MortekCrawler();

    auto legion = (Legion)GetEnumParam("Legion", parameters, None);
    unit->setLegion(legion);

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string MortekCrawler::ValueToString(const Parameter &parameter)
{
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int MortekCrawler::EnumStringToInt(const std::string &enumString)
{
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void MortekCrawler::Init()
{
    if (!s_registered)
    {
        static FactoryMethod factoryMethod = {
            MortekCrawler::Create,
            MortekCrawler::ValueToString,
            MortekCrawler::EnumStringToInt,
            MortekCrawler::ComputePoints,
            {
                {ParamType::Enum, "Legion", OssiarchBonereaperBase::None, OssiarchBonereaperBase::None, OssiarchBonereaperBase::Crematorians, 1},
            },
            DEATH,
            { OSSIARCH_BONEREAPERS }
        };
        s_registered = UnitFactory::Register("Mortek Crawler", factoryMethod);
    }
}

MortekCrawler::MortekCrawler() :
    OssiarchBonereaperBase("Mortek Crawler", 4, WOUNDS, 10, 4, false),
    m_catapultSkulls(Weapon::Type::Missile, "Dread Catapult - Necrotic Skulls", 36, 3, 2, 3, 0, 5),
    m_catapultCauldron(Weapon::Type::Missile, "Dread Catapult - Cauldron of Torment", 36, 1, 0, 0, 0, 0),
    m_catapultStele(Weapon::Type::Missile, "Dread Catapult - Cursed Stele", 36, 1, 0, 0, 0, 0),
    m_tools(Weapon::Type::Melee, "Crawler Tools", 1, 6, 3, 4, 0, 1)
{
    m_keywords = {DEATH, OSSIARCH_BONEREAPERS, WAR_MACHINE, MORTEK_CRAWLER};
    m_weapons = {&m_catapultSkulls, &m_catapultCauldron, &m_catapultSkulls, &m_tools};
}

bool MortekCrawler::configure()
{
    auto model = new Model(BASESIZE, wounds());
    model->addMissileWeapon(&m_catapultSkulls);
    model->addMissileWeapon(&m_catapultCauldron);
    model->addMissileWeapon(&m_catapultStele);
    model->addMeleeWeapon(&m_tools);

    m_catapultCauldron.activate(false);
    m_catapultStele.activate(false);

    addModel(model);

    m_points = POINTS_PER_UNIT;

    return true;
}

void MortekCrawler::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_catapultSkulls.setDamage(g_damageTable[damageIndex].m_skullDamage);

    Unit::onWounded();
}

int MortekCrawler::getDamageTableIndex() const
{
    auto woundsInflicted = wounds() - remainingWounds();
    for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++)
    {
        if (woundsInflicted < g_woundThresholds[i])
        {
            return i;
        }
    }
    return 0;
}

int MortekCrawler::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace OssiarchBonereapers
