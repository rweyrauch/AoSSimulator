/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordCorruptor.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Skaven {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 280;

    struct TableEntry {
        int m_move;
        int m_tailAttacks;
        int m_reaperAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {12, 4, 10},
                    {10, 3, 9},
                    {8,  2, 8},
                    {6,  1, 7},
                    {4,  0, 6}
            };


    bool VerminlordCorruptor::s_registered = false;

    Unit *VerminlordCorruptor::Create(const ParameterList &parameters) {
        auto unit = new VerminlordCorruptor();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void VerminlordCorruptor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Verminlord Corruptor", factoryMethod);
        }
    }

    VerminlordCorruptor::VerminlordCorruptor() :
            Skaventide("Verminlord Corruptor", 12, WOUNDS, 10, 4, false),
            m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
            m_plaguereapers(Weapon::Type::Melee, "Plaguereapers", 1, 10, 3, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, NURGLE, CLANS_PESTILENS, MONSTER, HERO,
                      WIZARD, VERMINLORD_CORRUPTOR};
        m_weapons = {&m_tails, &m_plaguereapers};

        s_globalBraveryMod.connect(this, &VerminlordCorruptor::terrifying, &m_connection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    VerminlordCorruptor::~VerminlordCorruptor() {
        m_connection.disconnect();
    }

    bool VerminlordCorruptor::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_tails);
        model->addMeleeWeapon(&m_plaguereapers);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds VerminlordCorruptor::applyWoundSave(const Wounds &wounds) {
        auto totalWounds = Skaventide::applyWoundSave(wounds);

        // Protection of the Horned Rat
        Dice::RollResult resultNormal, resultMortal;

        Dice::rollD6(wounds.normal, resultNormal);
        Dice::rollD6(wounds.mortal, resultMortal);

        Wounds negatedWounds = {resultNormal.rollsGE(5), resultNormal.rollsGE(5)};
        totalWounds -= negatedWounds;
        return totalWounds.clamp();
    }

    int VerminlordCorruptor::terrifying(const Unit *target) {
        // Terrifying
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    Wounds
    VerminlordCorruptor::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Plaguereapers
        if ((weapon->name() == m_plaguereapers.name()) && (hitRoll == 6)) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Wounds VerminlordCorruptor::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        // Plaguemaster
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto unit : units) {
            if (Dice::rollD6() >= 4) {
                Wounds plague = {0, Dice::rollD3()};
                unit->applyDamage(plague);
                wounds += plague;
            }
        }

        return wounds;
    }

    void VerminlordCorruptor::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_plaguereapers.setAttacks(g_damageTable[damageIndex].m_reaperAttacks);
    }

    void VerminlordCorruptor::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int VerminlordCorruptor::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int VerminlordCorruptor::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Skaven
