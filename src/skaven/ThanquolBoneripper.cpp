/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/ThanquolBoneripper.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <skaven/SkavenSpells.h>
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
        auto projectors = GetIntParam("Projectors", parameters, 2);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greySeerLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new ThanquolOnBoneripper(projectors, lore, general);
    }

    void ThanquolOnBoneripper::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Projectors", 2, 0, 4, 1),
                            EnumParameter("Lore", g_greySeerLore[0], g_greySeerLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Thanquol on Boneripper", factoryMethod);
        }
    }

    ThanquolOnBoneripper::ThanquolOnBoneripper(int numProjectors, Lore lore, bool isGeneral) :
            Skaventide("Thanquol on Boneripper", 10, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_projectors(Weapon::Type::Missile, "Warpfire Projectors", 8, 0, 0, 0, 0, 0),
            m_staff(Weapon::Type::Melee, "Staff of the Horned Rat", 2, 2, 4, 3, -1, RAND_D3),
            m_braziers(Weapon::Type::Melee, "Warpfire Braziers", 2, 0, 3, 3, -2, 3),
            m_blows(Weapon::Type::Melee, "Crushing Blows", 2, 6, 4, 3, -1, 2) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, MONSTER, HERO, WIZARD, GREY_SEER, THANQUOL};
        m_weapons = {&m_projectors, &m_staff, &m_braziers, &m_blows};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_blows.setMount(true);
        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        for (auto i = 0; i < numProjectors; i++)
            model->addMissileWeapon(&m_projectors);
        for (auto i = numProjectors - 1; i < 4; i++)
            model->addMeleeWeapon(&m_braziers);

        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_blows);
        addModel(model);

        //m_knownSpells.push_back(std::make_unique<Madness>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Wounds ThanquolOnBoneripper::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Skaventide::applyWoundSave(wounds, attackingUnit);

        // Protection of the Horned Rat
        int numSixes = 0;
        return ignoreWounds(totalWounds, 5, numSixes);
    }

    void ThanquolOnBoneripper::onWounded() {
        Skaventide::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_blows.setAttacks(g_damageTable[damageIndex].m_blowsAttacks);
    }

    void ThanquolOnBoneripper::onRestore() {
        Skaventide::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t ThanquolOnBoneripper::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int ThanquolOnBoneripper::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
