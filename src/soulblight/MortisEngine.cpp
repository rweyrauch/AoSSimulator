/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/MortisEngine.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 180;

    struct TableEntry {
        int m_move;
        double m_wailRange;
        int m_hostAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 12, 12},
                    {12, 10, 10},
                    {10, 8, 8},
                    {8,  6, 6},
                    {6,  4, 4}
            };

    bool MortisEngine::s_registered = false;

    Unit *MortisEngine::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        return new MortisEngine(bloodline);
    }

    int MortisEngine::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void MortisEngine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Mortis Engine", factoryMethod);
        }
    }

    MortisEngine::MortisEngine(CursedBloodline bloodline) :
            SoulblightBase(bloodline, "Mortis Engine", 14, g_wounds, 10, 4, true, g_pointsPerUnit) {
        m_keywords = {DEATH, MALIGNANT, SOULBLIGHT_GRAVELORDS, DEATHMAGES, MORTIS_ENGINE};
        m_weapons = {&m_wail, &m_staff, &m_etherealWeapons};
        m_battleFieldRole = Role::Behemoth;

        s_globalCastMod.connect(this, &MortisEngine::boundNecromancer, &m_boundNecromancer);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_wail);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_etherealWeapons);
        addModel(model);
    }

    MortisEngine::~MortisEngine() {
        m_boundNecromancer.disconnect();
    }

    void MortisEngine::onWounded() {
        SoulblightBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_etherealWeapons.setAttacks(g_damageTable[damageIndex].m_hostAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void MortisEngine::onRestore() {
        SoulblightBase::onRestore();

        m_usedReliquary = false;

        // Restore table-driven attributes
        onWounded();
    }

    size_t MortisEngine::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds MortisEngine::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_etherealWeapons.name())) return {0, 1};
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void MortisEngine::onStartShooting(PlayerId player) {
        SoulblightBase::onStartShooting(player);

        // Wail of the Damned
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()),
                                                       g_damageTable[getDamageTableIndex()].m_wailRange);
        for (auto unit : units) {
            if (Dice::RollD6() >= 4) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    void MortisEngine::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);

        if (owningPlayer() == player) {
            // The Reliquary
            if (!m_usedReliquary) {
                auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 12.0);
                if (!units.empty()) {
                    for (auto unit : units) {
                        if (unit->hasKeyword(DEATH)) continue;

                        m_usedReliquary = true;
                        if (Dice::RollD6() >= 2) {
                            unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability, nullptr}, this);
                        }
                    }
                }
            }
        }
    }

    int MortisEngine::boundNecromancer(const Unit *caster) {
        if (isFriendly(caster) && caster->hasKeyword(SOULBLIGHT_GRAVELORDS) && (distanceTo(caster) < 12.0)) {
            return 1;
        }
        return 0;
    }

} // namespace Soulblight
