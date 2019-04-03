/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <eldritchcouncil/ArchmageOnDragon.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace EldritchCouncil
{
struct TableEntry
{
    int m_move;
    int m_clawAttacks;
    int m_jawsToHit;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, ArchmageOnDragon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {14, 6, 2},
        {12, 5, 3},
        {10, 4, 3},
        {8,  3, 4},
        {6,  2, 4}
    };

static FactoryMethod factoryMethod = {
    ArchmageOnDragon::Create,
    nullptr,
    nullptr,
    {
        {ParamType::Boolean, "Arcane Tome", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
        {ParamType::Boolean, "Talisman of Arcane Power", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
    },
    ORDER,
    ELDRITCH_COUNCIL
};

bool ArchmageOnDragon::s_registered = false;

ArchmageOnDragon::ArchmageOnDragon() :
    Unit("Archmage On Dragon", 14, WOUNDS, 7, 5, true),
    m_magestaff(Weapon::Type::Melee, "Magestaff", 2, 1, 4, 3, -1, RAND_D3),
    m_sorcerousBlade(Weapon::Type::Melee, "Sorcerous Blade", 1, 3, 4, 4, 0, 1),
    m_dragonClaws(Weapon::Type::Melee, "Dragon's Claws", 2, 6, 4, 3, -1, 2),
    m_dragonJaws(Weapon::Type::Melee, "Dragon's Fearsome Jaws", 3, 3, 4, 2, -2, RAND_D6)
{
    m_keywords = {ORDER, AELF, DRAGON, ELDRITCH_COUNCIL, HERO, WIZARD, MONSTER, ARCHMAGE};
    m_totalUnbinds = 1;
    m_totalSpells = 1;
}

bool ArchmageOnDragon::configure(bool arcaneTome, bool talisman)
{
    m_arcaneTome = arcaneTome;
    m_talismanOfArcanePower = talisman;

    if (m_arcaneTome)
    {
        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    Model model;
    model.addMeleeWeapon(&m_magestaff);
    if (!m_arcaneTome)
    {
        model.addMeleeWeapon(&m_sorcerousBlade);
    }
    model.addMeleeWeapon(&m_dragonClaws);
    model.addMeleeWeapon(&m_dragonJaws);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void ArchmageOnDragon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_magestaff);
    if (!m_arcaneTome)
    {
        visitor(&m_sorcerousBlade);
    }
    visitor(&m_dragonClaws);
    visitor(&m_dragonJaws);
}

Unit *ArchmageOnDragon::Create(const ParameterList &parameters)
{
    auto unit = new ArchmageOnDragon();
    bool tome = GetBoolParam("Arcane Tome", parameters, false);
    bool talisman = GetBoolParam("Talisman of Arcane Power", parameters, false);

    bool ok = unit->configure(tome, talisman);
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

int ArchmageOnDragon::unbindingModifier() const
{
    int modifier = Unit::unbindingModifier();
    // Talisman of Arcane Power
    if (m_talismanOfArcanePower)
        modifier += 1;
    return modifier;
}

void ArchmageOnDragon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Archmage On Dragon", factoryMethod);
    }
}

int ArchmageOnDragon::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

void ArchmageOnDragon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_dragonClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
    m_dragonJaws.setToHit(g_damageTable[damageIndex].m_jawsToHit);
}

int ArchmageOnDragon::getDamageTableIndex() const
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

} //namespace EldritchCouncil