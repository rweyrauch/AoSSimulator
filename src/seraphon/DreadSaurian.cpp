/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <seraphon/DreadSaurian.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 280; // x210 oval
    static const int g_wounds = 35;
    static const int g_pointsPerUnit = 510;

    bool DreadSaurian::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_jawsDamage;
        int m_clawsToHit;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {12, 18, 24, 30, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 6, 2},
                    {9,  5, 3},
                    {8,  4, 3},
                    {7,  3, 4},
                    {6,  2, 5}
            };

    DreadSaurian::DreadSaurian(WayOfTheSeraphon way, Constellation constellation) :
            SeraphonBase("Dread Saurian", 10, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {ORDER, SERAPHON, MONSTER, TOTEM, DREAD_SAURIAN};
        m_weapons = {&m_gargantuanJaws, &m_rakingClaws, &m_armouredTail};
        m_battleFieldRole = Role::Behemoth;

        setWayOfTheSeraphon(way, constellation);

        s_globalBraveryMod.connect(this, &DreadSaurian::terror, &m_terrorSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_gargantuanJaws);
        model->addMeleeWeapon(&m_rakingClaws);
        model->addMeleeWeapon(&m_armouredTail);
        addModel(model);
    }

    DreadSaurian::~DreadSaurian() {
        m_terrorSlot.disconnect();
    }

    void DreadSaurian::onRestore() {
        SeraphonBase::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    Unit *DreadSaurian::Create(const ParameterList &parameters) {
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        return new DreadSaurian(way, constellation);
    }

    void DreadSaurian::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Dread Saurian", factoryMethod);
        }
    }

    void DreadSaurian::onWounded() {
        SeraphonBase::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_rakingClaws.setToHit(g_damageTable[damageIndex].m_clawsToHit);
        m_gargantuanJaws.setDamage(g_damageTable[damageIndex].m_jawsDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t DreadSaurian::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int DreadSaurian::terror(const Unit *target) {
        // Terror
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    void DreadSaurian::onCharged() {
        SeraphonBase::onCharged();

        // Obliterating Charge
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto unit : units) {
            if (Dice::RollD6() >= 2) {
                if (unit->hasKeyword(MONSTER))
                    unit->applyDamage({0, Dice::RollD3()}, this);
                else
                    unit->applyDamage({0, Dice::RollD6()}, this);
            }
        }
    }

    void DreadSaurian::onFriendlyUnitSlain(const Unit *attacker) {
        SeraphonBase::onFriendlyUnitSlain(nullptr);

        // Death Throes
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto unit : units) {
            if (!unit->hasKeyword(MONSTER)) {
                if (Dice::RollD6() >= 4)
                    unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

    int DreadSaurian::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon