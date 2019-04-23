/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <everchosen/Archaon.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace Everchosen
{
static FactoryMethod factoryMethod = {
    Archaon::Create,
    nullptr,
    nullptr,
    {
    },
    CHAOS,
    EVERCHOSEN
};

struct TableEntry
{
    int m_move;
    int m_clawsToHit;
    int m_headsAttacks;
};

const size_t NUM_TABLE_ENTRIES = 5;
static int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 8, 12, 16, Archaon::WOUNDS};
static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
    {
        {12, 2, 6},
        {10, 3, 5},
        {8,  4, 4},
        {7,  4, 3},
        {6,  5, 2}
    };

bool Archaon::s_registered = false;

Archaon::Archaon() :
    Unit("Archaon", 12, WOUNDS, 10, 3, true),
    m_slayerOfKings(Weapon::Type::Melee, "The Slayer of Kings", 1, 4, 2, 3, -1, 3),
    m_dorgharsClaws(Weapon::Type::Melee, "Dorghar's Monstrous Claws", 1, 2, 2, 3, -1, RAND_D6),
    m_dorgharsTails(Weapon::Type::Melee, "Dorghar's Lashing Tails", 3, RAND_2D6, 4, 3, 0, 1),
    m_dorgharsHeads(Weapon::Type::Melee, "Dorghar's Three Heads", 3, 6, 4, 3, -1, 1)
{
    m_keywords = {CHAOS, DAEMON, MORTAL, KHORNE, NURGLE, SLAANESH, TZEENTCH, EVERCHOSEN, MONSTER, HERO, WIZARD, ARCHAON};

    m_totalUnbinds = 2;
    m_totalSpells = 2;
}

bool Archaon::configure()
{
    Model model(BASESIZE, WOUNDS);

    model.addMeleeWeapon(&m_slayerOfKings);
    model.addMeleeWeapon(&m_dorgharsClaws);
    model.addMeleeWeapon(&m_dorgharsTails);
    model.addMeleeWeapon(&m_dorgharsHeads);
    addModel(model);

    m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
    m_knownSpells.push_back(std::make_unique<MysticShield>(this));

    m_points = POINTS_PER_UNIT;

    return true;
}

void Archaon::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_slayerOfKings);
    visitor(&m_dorgharsClaws);
    visitor(&m_dorgharsTails);
    visitor(&m_dorgharsHeads);
}

int Archaon::move() const
{
    return g_damageTable[getDamageTableIndex()].m_move;
}

Unit *Archaon::Create(const ParameterList &parameters)
{
    auto unit = new Archaon();

    bool ok = unit->configure();
    if (!ok)
    {
        delete unit;
        unit = nullptr;
    }
    return unit;
}

void Archaon::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Archaon", factoryMethod);
    }
}

void Archaon::onWounded()
{
    const int damageIndex = getDamageTableIndex();
    m_dorgharsClaws.setToHit(g_damageTable[damageIndex].m_clawsToHit);
    m_dorgharsHeads.setAttacks(g_damageTable[damageIndex].m_headsAttacks);
}

int Archaon::getDamageTableIndex() const
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

Wounds Archaon::computeReturnedDamage(const Weapon *weapon, int saveRoll) const
{
    // The Armour of Morkar
    if (saveRoll == 6)
    {
        return {0, 1};
    }
    return Unit::computeReturnedDamage(weapon, saveRoll);
}

Wounds Archaon::applyWoundSave(const Wounds &wounds)
{
    auto modifiedWounds = Unit::applyWoundSave(wounds);

    // Chaos Runeshield
    Dice dice;
    Dice::RollResult rollResult;

    dice.rollD6(wounds.normal, rollResult);
    modifiedWounds.normal -= rollResult.rollsGE(5);
    dice.rollD6(wounds.mortal, rollResult);
    modifiedWounds.mortal -= rollResult.rollsGE(5);

    return modifiedWounds;
}

Wounds Archaon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const
{
    auto wounds = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    // The Slayer of Kings
    if ((hitRoll == 6) && (weapon->name() == m_slayerOfKings.name()))
    {
        m_slayerOfKingsSixesThisCombat++;

        if (m_slayerOfKingsSixesThisCombat >= 2)
        {
            // TODO: Hack for now.  Expect next revision of Archaon warscroll to remove the auto-slay and instead
            // do some large number of mortal wounds (just like Skarbrand).
            // Target is slain.  (Fake it by generating a lot of mortal wounds).
            wounds = {weapon->damage(), target->wounds()*10};
        }
    }
    return wounds;
}

void Archaon::onStartCombat(PlayerId player)
{
    Unit::onStartCombat(player);

    m_slayerOfKingsSixesThisCombat = 0;
}

} // namespace Everchosen
