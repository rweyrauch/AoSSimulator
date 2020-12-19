/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/Gordrakk.h>
#include <UnitFactory.h>
#include <Board.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int g_basesize = 160;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 540;

    bool GordrakkTheFistOfGork::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_fistsAttacks;
        int m_bulkDice;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 10, 13, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 9, 9},
                    {10, 8, 8},
                    {8,  7, 7},
                    {6,  6, 6},
                    {4,  5, 5}
            };

    GordrakkTheFistOfGork::GordrakkTheFistOfGork() :
            Ironjawz("Gordrakk the Fist of Gork", 12, g_wounds, 8, 3, true),
            m_bellow(Weapon::Type::Missile, "Innard-bursting Bellow", 8, 6, 2, 3, -1, 1),
            m_smasha(Weapon::Type::Melee, "Smasha", 1, 5, 2, 3, -1, RAND_D3),
            m_kunnin(Weapon::Type::Melee, "Kunnin'", 1, 5, 2, 3, -1, 1),
            m_fistsAndTail(Weapon::Type::Melee, "Mighty Fists and Tail", 1, 9, 3, 3, -2, 2) {
        m_keywords = {DESTRUCTION, ORRUK, MAW_KRUSHA, IRONJAWZ, MONSTER, HERO, MEGABOSS, GORDRAKK};
        m_weapons = {&m_bellow, &m_smasha, &m_kunnin, &m_fistsAndTail};
        m_hasMount = true;
        m_battleFieldRole = Leader_Behemoth;
    }

    bool GordrakkTheFistOfGork::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bellow);
        model->addMeleeWeapon(&m_smasha);
        model->addMeleeWeapon(&m_kunnin);
        model->addMeleeWeapon(&m_fistsAndTail);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *GordrakkTheFistOfGork::Create(const ParameterList &parameters) {
        auto unit = new GordrakkTheFistOfGork();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GordrakkTheFistOfGork::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GordrakkTheFistOfGork::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    GordrakkTheFistOfGork::ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Gordrakk the Fist of Gork", factoryMethod);
        }
    }

    void GordrakkTheFistOfGork::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    void GordrakkTheFistOfGork::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_fistsAndTail.setAttacks(g_damageTable[damageIndex].m_fistsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int GordrakkTheFistOfGork::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void GordrakkTheFistOfGork::onCharged() {
        Unit::onCharged();

        // Massively Destructive Bulk
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        if (!units.empty()) {
            auto unit = units.front();
            Dice::RollResult result;
            Dice::RollD6(g_damageTable[getDamageTableIndex()].m_bulkDice, result);
            Wounds bulkWounds = {0, result.rollsGE(5)};
            unit->applyDamage(bulkWounds, this);
        }
    }

    Wounds
    GordrakkTheFistOfGork::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if ((woundRoll >= 4) && (weapon->name() == m_kunnin.name())) {
            if (target->hasKeyword(WIZARD)) {
                return {0, Dice::RollD3()};
            }
        } else if ((woundRoll >= 4) && (weapon->name() == m_smasha.name())) {
            if (target->hasKeyword(HERO) && !target->hasKeyword(WIZARD)) {
                return {0, Dice::RollD3()};
            }
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void GordrakkTheFistOfGork::onStartCombat(PlayerId player) {
        m_modelsSlainAtStartOfCombat = m_currentRecord.m_enemyModelsSlain;

        Ironjawz::onStartCombat(player);
    }

    Wounds GordrakkTheFistOfGork::onEndCombat(PlayerId player) {
        // Strength from Victory
        if (m_currentRecord.m_enemyModelsSlain > m_modelsSlainAtStartOfCombat) {
            heal(1);
            m_smasha.setAttacks(m_smasha.attacks() + 1);
            m_kunnin.setAttacks(m_kunnin.attacks() + 1);
        }
        return Ironjawz::onEndCombat(player);
    }

    int GordrakkTheFistOfGork::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Ironjawz