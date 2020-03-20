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
static const int BASESIZE = 105;
static const int WOUNDS = 11;
static const int POINTS_PER_UNIT = 360;

struct TableEntry
{
    int m_move;
    int m_clawAttacks;
    int m_staffCast;
    int m_staffUnbind;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {16, 6, 2, 2},
        {13, 5, 2, 1},
        {10, 4, 1, 1},
        {7,  3, 1, 0},
        {4,  2, 0, 0}
    };

bool ArkhanTheBlack::s_registered = false;

Unit *ArkhanTheBlack::Create(const ParameterList &parameters)
{
    auto unit = new ArkhanTheBlack();

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
        static FactoryMethod factoryMethod = {
            ArkhanTheBlack::Create,
            ArkhanTheBlack::ValueToString,
            ArkhanTheBlack::EnumStringToInt,
            ArkhanTheBlack::ComputePoints,
            {
                {ParamType::Enum, "Legion", OssiarchBonereaperBase::None, OssiarchBonereaperBase::None, OssiarchBonereaperBase::Crematorians, 1},
            },
            DEATH,
            { OSSIARCH_BONEREAPERS, DEATHLORDS }
        };
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
    auto model = new Model(BASESIZE, wounds());
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
    m_move = g_damageTable[getDamageTableIndex()].m_move;

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

void ArkhanTheBlack::onRestore()
{
    Unit::onRestore();

    // Restore table-driven attributes
    onWounded();
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

int ArkhanTheBlack::castingModifier() const
{
    auto mod = Unit::castingModifier();

    mod += g_damageTable[getDamageTableIndex()].m_staffCast;

    return mod;
}

int ArkhanTheBlack::unbindingModifier() const
{
    auto mod = Unit::unbindingModifier();

    mod += g_damageTable[getDamageTableIndex()].m_staffUnbind;

    return mod;
}

int ArkhanTheBlack::ComputePoints(int numModels)
{
    return POINTS_PER_UNIT;
}

} // namespace OssiarchBonereapers
