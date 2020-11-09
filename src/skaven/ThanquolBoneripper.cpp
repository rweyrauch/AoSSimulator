/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/ThanquolBoneripper.h>
#include <UnitFactory.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 390;

    struct TableEntry {
        int m_move;
        int m_blowsAttacks;
        int m_staffCastBuff;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 5, 8, 10, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 6, 2},
                    {9,  5, 2},
                    {8,  4, 1},
                    {7,  3, 1},
                    {6,  2, 0}
            };


    bool ThanquolOnBoneripper::s_registered = false;

    Unit *ThanquolOnBoneripper::Create(const ParameterList &parameters) {
        auto unit = new ThanquolOnBoneripper();
        int numProjectors = 0;

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greySeerLore[0]);

        bool ok = unit->configure(numProjectors, lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ThanquolOnBoneripper::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                        EnumParameter("Lore", g_greySeerLore[0], g_greySeerLore),
                        BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Thanquol on Boneripper", factoryMethod);
        }
    }

    ThanquolOnBoneripper::ThanquolOnBoneripper() :
            Skaventide("Thanquol on Boneripper", 10, g_wounds, 7, 4, false),
            m_projectors(Weapon::Type::Missile, "Warpfire Projectors", 8, 0, 0, 0, 0, 0),
            m_staff(Weapon::Type::Melee, "Staff of the Horned Rat", 2, 2, 4, 3, -1, RAND_D3),
            m_braziers(Weapon::Type::Melee, "Warpfire Braziers", 2, 0, 3, 3, -2, 3),
            m_blows(Weapon::Type::Melee, "Crushing Blows", 2, 6, 4, 3, -1, 2) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, MONSTER, HERO, WIZARD, GREY_SEER, THANQUOL};
        m_weapons = {&m_projectors, &m_staff, &m_braziers, &m_blows};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool ThanquolOnBoneripper::configure(int numProjectors, Lore lore) {
        auto model = new Model(g_basesize, wounds());

        for (auto i = 0; i < numProjectors; i++)
            model->addMissileWeapon(&m_projectors);
        for (auto i = numProjectors - 1; i < 4; i++)
            model->addMeleeWeapon(&m_braziers);

        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_blows);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds ThanquolOnBoneripper::applyWoundSave(const Wounds &wounds) {
        auto totalWounds = Skaventide::applyWoundSave(wounds);

        // Protection of the Horned Rat
        Dice::RollResult resultNormal, resultMortal;

        Dice::RollD6(wounds.normal, resultNormal);
        Dice::RollD6(wounds.mortal, resultMortal);

        Wounds negatedWounds = {resultNormal.rollsGE(5), resultNormal.rollsGE(5)};
        totalWounds -= negatedWounds;
        return totalWounds.clamp();
    }

    void ThanquolOnBoneripper::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_blows.setAttacks(g_damageTable[damageIndex].m_blowsAttacks);
    }

    void ThanquolOnBoneripper::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int ThanquolOnBoneripper::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int ThanquolOnBoneripper::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
