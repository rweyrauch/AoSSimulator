/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <beastsofchaos/ChaosGargant.h>
#include <UnitFactory.h>
#include <Board.h>

namespace BeastsOfChaos {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 180;

    struct TableEntry {
        int m_move;
        int m_clubAttacks;
        int m_eadbuttDamage;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, RAND_3D6, 6},
                    {6, RAND_2D6, RAND_D6},
                    {5, RAND_2D6, RAND_D3},
                    {4, RAND_D6,  RAND_D3},
                    {3, RAND_D6,  1}
            };

    bool ChaosGargant::s_registered = false;

    ChaosGargant::ChaosGargant() :
            BeastsOfChaosBase("Chaos Gargant", 8, g_wounds, 6, 5, false),
            m_eadbutt(Weapon::Type::Melee, "Vicious 'eadbutt", 1, 1, 4, 3, -3, 6),
            m_massiveClub(Weapon::Type::Melee, "Massive Club", 3, RAND_3D6, 3, 3, -1, 1),
            m_mightyKick(Weapon::Type::Melee, "Mighty Kick", 2, 1, 3, 3, -2, RAND_D3) {
        m_keywords = {CHAOS, GARGANT, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, CHAOS_GARGANT};
        m_weapons = {&m_eadbutt, &m_massiveClub, &m_mightyKick};
        m_battleFieldRole = Role::Behemoth;
    }

    bool ChaosGargant::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_eadbutt);
        model->addMeleeWeapon(&m_massiveClub);
        model->addMeleeWeapon(&m_mightyKick);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void ChaosGargant::onWounded() {
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_eadbutt.setDamage(g_damageTable[getDamageTableIndex()].m_eadbuttDamage);
        m_massiveClub.setAttacks(g_damageTable[getDamageTableIndex()].m_clubAttacks);
    }

    Unit *ChaosGargant::Create(const ParameterList &parameters) {
        auto unit = new ChaosGargant();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosGargant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosGargant::Create,
                    nullptr,
                    nullptr,
                    ChaosGargant::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Chaos Gargant", factoryMethod);
        }
    }

    int ChaosGargant::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void ChaosGargant::onRestore() {
        Unit::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    int ChaosGargant::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void ChaosGargant::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Whipped into a Frenzy
        if (remainingWounds() > wounds() / 2) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
            bool heroesNearby = false;
            for (auto unit : units) {
                if (unit->hasKeyword(HERO) && unit->hasKeyword(BEASTS_OF_CHAOS)) {
                    heroesNearby = true;
                    break;
                }
            }
            if (heroesNearby) {
                applyDamage({0, 1}, this);
                buffModifier(Attribute::Attacks_Melee, 1, {Phase::Combat, m_battleRound, player});
            }
        }

        // Stuff'Em In Me Bag
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 3.0)) {
            if (Dice::RollD6() >= unit->wounds() * 2) {
                unit->slay(1);
            }
        }
    }

} // namespace GloomspiteGitz
