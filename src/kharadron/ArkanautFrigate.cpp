/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/ArkanautFrigate.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>

namespace KharadronOverlords
{
static FactoryMethod factoryMethod = {
    ArkanautFrigate::Create,
    ArkanautFrigate::ValueToString,
    ArkanautFrigate::EnumStringToInt,
    ArkanautFrigate::ComputePoints,
    {
        {
            ParamType::Enum, "Weapon", ArkanautFrigate::HeavySkyCannon, ArkanautFrigate::HeavySkyCannon,
            ArkanautFrigate::HeavySkyhook, 1
        },
        {ParamType::Enum, "Skyport", KharadronBase::None, KharadronBase::None, KharadronBase::Custom, 1},
    },
    ORDER,
    { KHARADRON_OVERLORDS }
};

struct TableEntry
{
    int m_move;
    int m_boardingAttacks;
    int m_bombRacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, ArkanautFrigate::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 6, 1},
        {10, 5, 0},
        {8, 4, 0},
        {6, 3, -1},
        {4, 2, -1}
    };

bool ArkanautFrigate::s_registered = false;

Unit *ArkanautFrigate::Create(const ParameterList &parameters)
{
    auto unit = new ArkanautFrigate();
    auto option = (WeaponOption)GetEnumParam("Weapon", parameters, HeavySkyCannon);

    auto port = (Skyport)GetEnumParam("Skyport", parameters, KharadronBase::None);
    unit->setSkyport(port);

    bool ok = unit->configure(option);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

std::string ArkanautFrigate::ValueToString(const Parameter &parameter)
{
    if (std::string(parameter.name) == "Weapon")
    {
        if (parameter.intValue == HeavySkyCannon) return "Heavy Sky Cannon";
        else if (parameter.intValue == HeavySkyhook) return "Heavy Skyhook";
    }
    return KharadronBase::ValueToString(parameter);
}

int ArkanautFrigate::EnumStringToInt(const std::string &enumString)
{
    if (enumString == "Heavy Sky Cannon") return HeavySkyCannon;
    else if (enumString == "Heavy Skyhook") return HeavySkyhook;
    return KharadronBase::EnumStringToInt(enumString);
}

void ArkanautFrigate::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Arkanaut Frigate", factoryMethod);
    }
}

ArkanautFrigate::ArkanautFrigate() :
    KharadronBase("Arkanaut Frigate", 12, WOUNDS, 8, 4, true),
    m_cannonShrapnel(Weapon::Type::Missile, "Heavy Sky Cannon: Shrapnel", 24, RAND_D6, 3, 3, -1, 2),
    m_cannonShell(Weapon::Type::Missile, "Heavy Sky Cannon: Shell", 30, 1, 3, 2, -2, RAND_D6),
    m_skyhook(Weapon::Type::Missile, "Heavy Skyhook", 24, 1, 3, 2, -2, RAND_D6),
    m_carbines(Weapon::Type::Missile, "Aethershot Carbines", 12, 4, 3, 3, -1, 2),
    m_boardingWeapons(Weapon::Type::Melee, "Boarding Weapons", 1, 6, 4, 4, 0, 1)
{
    m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, WAR_MACHINE, SKYVESSEL, ARKANAUT_FRIGATE};
    m_weapons = {&m_cannonShrapnel,
                 &m_cannonShell,
                 &m_skyhook,
                 &m_carbines,
                 &m_boardingWeapons};
}

bool ArkanautFrigate::configure(WeaponOption option)
{
    auto model = new Model(BASESIZE, WOUNDS);
    if (option == HeavySkyCannon)
    {
        model->addMissileWeapon(&m_cannonShrapnel);
        model->addMissileWeapon(&m_cannonShell);
        m_cannonShell.activate(false);
    }
    else if (option == HeavySkyhook)
    {
        model->addMissileWeapon(&m_skyhook);
    }
    model->addMissileWeapon(&m_carbines);
    model->addMeleeWeapon(&m_boardingWeapons);
    addModel(model);

    m_weaponOption = option;

    m_points = POINTS_PER_UNIT;

    return true;
}

void ArkanautFrigate::onRestore()
{
    // Restore table-driven attributes
    onWounded();
}

void ArkanautFrigate::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_boardingWeapons.setAttacks(g_damageTable[damageIndex].m_boardingAttacks);
    m_move = g_damageTable[getDamageTableIndex()].m_move;
}

int ArkanautFrigate::getDamageTableIndex() const
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

void ArkanautFrigate::onStartShooting(PlayerId player)
{
    Unit::onStartShooting(player);

    if (m_weaponOption == HeavySkyCannon)
    {
        auto nearestUnit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (nearestUnit)
        {
            const auto preferShell = (nearestUnit->save() < 4);
            m_cannonShell.activate(preferShell);
            m_cannonShrapnel.activate(!preferShell);
        }
    }
}

int ArkanautFrigate::chargeModifier() const
{
    auto mod = Unit::chargeModifier();

    if (m_weaponOption == HeavySkyhook) mod += 2;

    return mod;
}

void ArkanautFrigate::onStartHero(PlayerId player)
{
    Unit::onStartHero(player);

    // Aetheric Navigator/Endrinrigger
    if (player == owningPlayer())
    {
        m_models.front()->applyHealing(1);
    }

}

Rerolls ArkanautFrigate::runRerolls() const
{
    // Aetheric Navigator/Endrinrigger
    return RerollFailed;
}

void ArkanautFrigate::onStartCombat(PlayerId player)
{
    Unit::onStartCombat(player);

    // Bomb Racks
    auto nearestUnit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
    if (nearestUnit && (distanceTo(nearestUnit) <= 1.0f))
    {
        auto roll = Dice::rollD6();
        roll += g_damageTable[getDamageTableIndex()].m_bombRacks;

        if (roll >= 4)
        {
            int wounds = Dice::rollD3();
            nearestUnit->applyDamage({0, wounds});
        }
    }
}

} //KharadronOverlords
