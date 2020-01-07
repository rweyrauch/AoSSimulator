/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/ArkhanTheBlack.h"

namespace OssiarchBonereapers
{
static FactoryMethod factoryMethod = {
    ArkhanTheBlack::Create,
    ArkhanTheBlack::ValueToString,
    ArkhanTheBlack::EnumStringToInt,
    {
    },
    DEATH,
    OSSIARCH_BONEREAPERS
};

struct TableEntry
{
    int m_move;
    int m_clawAttacks;
    int m_staff;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, ArkhanTheBlack::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {16, 6, 4},
        {13, 5, 3},
        {10, 4, 1},
        {7,  3, 1},
        {4,  2, 0}
    };

bool ArkhanTheBlack::s_registered = false;

Unit *ArkhanTheBlack::Create(const ParameterList &parameters)
{
    auto unit = new ArkhanTheBlack();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string ArkhanTheBlack::ValueToString(const Parameter &parameter)
{
    return OssiarchBonereaperBase::ValueToString(parameter);
}

int ArkhanTheBlack::EnumStringToInt(const std::string &enumString)
{
    return OssiarchBonereaperBase::EnumStringToInt(enumString);
}

void ArkhanTheBlack::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Arkhan the Black, Mortarch of Sacrament", factoryMethod);
    }
}

ArkhanTheBlack::ArkhanTheBlack() :
    OssiarchBonereaperBase("Arkhan the Black, Mortarch of Sacrament", 16, WOUNDS, 10, 4, true),
    m_zefetKar(Weapon::Type::Melee, "Zefet-kar", 1, 1, 3, 3, -1, RAND_D3),
    m_khenashAn(Weapon::Type::Melee, "Khenash-an", 2, 1, 4, 3, -1, RAND_D3),
    m_claws(Weapon::Type::Melee, "Ebon Claws", 1, 6, 4, 3, -2, 2),
    m_clawsAndDaggers(Weapon::Type::Melee, "Spectral Claws and Dagger", 1, 6, 5, 4, 0, 1)
{
    m_keywords = {DEATH, SKELETON, DEATHLORDS, MONSTER, HERO, WIZARD, MORTARCH, ARKHAN};
    m_weapons = {&m_zefetKar, &m_khenashAn, &m_claws, &m_clawsAndDaggers};

    m_totalSpells = 3;
    m_totalUnbinds = 3;
}

bool ArkhanTheBlack::configure()
{
    auto model = new Model(BASESIZE, WOUNDS);
    model->addMeleeWeapon(&m_zefetKar);
    model->addMeleeWeapon(&m_khenashAn);
    model->addMeleeWeapon(&m_claws);
    model->addMeleeWeapon(&m_clawsAndDaggers);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void ArkhanTheBlack::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);

    Unit::onWounded();
}

int ArkhanTheBlack::getDamageTableIndex() const
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

int ArkhanTheBlack::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Wounds ArkhanTheBlack::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    // Frightful Touch
    if ((hitRoll == 6) && (weapon->name() == m_clawsAndDaggers.name()))
    {
        return {0, 1};
    }
    return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
}

} // namespace OssiarchBonereapers