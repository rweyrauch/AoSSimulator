/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/RogueIdol.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Greenskinz {
    static const int g_basesize = 170; // x105 oval
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 420;

    struct TableEntry {
        int m_move;
        int m_fistToWound;
        int m_feetAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {4, 8, 11, 13, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 2, 10},
                    {8,  3, 8},
                    {6,  3, 6},
                    {4,  4, 4},
                    {2,  5, 2}
            };

    bool RogueIdol::s_registered = false;

    RogueIdol::RogueIdol() :
            Unit("Rogue Idol", 10, g_wounds, 10, 4, false),
            m_boulderFists(Weapon::Type::Melee, "Boulder Fists", 3, 2, 3, 2, -2, RAND_D6),
            m_stompinFeet(Weapon::Type::Melee, "Stompin' Feet", 2, 10, 3, 3, -2, 2) {
        m_keywords = {DESTRUCTION, GREENSKINZ, MONSTER, ROGUE_IDOL};
        m_weapons = {&m_boulderFists, &m_stompinFeet};
        m_battleFieldRole = Role::Behemoth;
    }

    void RogueIdol::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_boulderFists);
        model->addMeleeWeapon(&m_stompinFeet);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *RogueIdol::Create(const ParameterList &parameters) {
        auto unit = new RogueIdol();

        unit->configure();
        return unit;
    }

    void RogueIdol::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    RogueIdol::Create,
                    nullptr,
                    nullptr,
                    RogueIdol::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GREENSKINZ}
            };
            s_registered = UnitFactory::Register("Rogue Idol", factoryMethod);
        }
    }

    void RogueIdol::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    size_t RogueIdol::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void RogueIdol::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_boulderFists.setToWound(g_damageTable[damageIndex].m_fistToWound);
        m_stompinFeet.setAttacks(g_damageTable[damageIndex].m_feetAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        Unit::onWounded();
    }

    void RogueIdol::onFriendlyUnitSlain(const Unit *attacker) {
        // Avalanche!
        auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= 4) {
                ip->applyDamage({0, Dice::RollD3()}, this);
            }
        }

        Unit::onFriendlyUnitSlain(nullptr);
    }

    Wounds RogueIdol::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        Wounds modifiedWounds = wounds;

        // Da Big' Un
        modifiedWounds.normal = (wounds.normal + 1) / 2;
        if (wounds.mortal > 0) {
            Dice::RollResult rolls;
            Dice::RollD6(wounds.mortal, rolls);
            modifiedWounds.mortal = rolls.rollsGE(4);
        }
        return modifiedWounds;
    }

    void RogueIdol::onEndCombat(PlayerId player) {
        Unit::onEndCombat(player);

        // Rubble and Ruin
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto ip : units) {
            int roll = Dice::RollD6();
            if (roll >= 4) {
                Wounds rubbleRuins = {0, 0};
                ip->applyDamage(rubbleRuins, this);
            }
        }
    }

    Rerolls RogueIdol::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Spirit of the Waaagh!
        if (m_charged) {
            return Rerolls::Ones;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int RogueIdol::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Greenskinz
