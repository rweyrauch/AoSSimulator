/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/LordSkreech.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <skaven/SkavenSpells.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 320;

    struct TableEntry {
        int m_move;
        int m_tailAttacks;
        int m_reaperAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 4, 8},
                    {10, 3, 7},
                    {8,  2, 6},
                    {6,  1, 5},
                    {4,  0, 4}
            };


    bool LordSkreechVerminking::s_registered = false;

    Unit *LordSkreechVerminking::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_skryreLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LordSkreechVerminking(lore, general);
    }

    void LordSkreechVerminking::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore", g_skryreLore[0], g_skryreLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Lord Skreech Verminking", factoryMethod);
        }
    }

    LordSkreechVerminking::LordSkreechVerminking(Lore lore, bool isGeneral) :
            Skaventide("Lord Skreech Verminking", 12, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
            m_glaive(Weapon::Type::Melee, "Doom Glaive", 3, 6, 3, 3, -1, RAND_D3),
            m_plaguereaper(Weapon::Type::Melee, "Plaguereaper", 1, 8, 3, 3, -1, 1) {
        m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, MASTERCLAN, MONSTER, HERO, WIZARD,
                      LORD_SKREECH_VERMINKING};
        m_weapons = {&m_tails, &m_glaive, &m_plaguereaper};
        m_battleFieldRole = Role::Leader_Behemoth;

        s_globalBraveryMod.connect(this, &LordSkreechVerminking::terrifying, &m_connection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_tails);
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_plaguereaper);
        addModel(model);

        //m_knownSpells.push_back(std::make_unique<DreadedThirteenthSpell>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    LordSkreechVerminking::~LordSkreechVerminking() {
        m_connection.disconnect();
    }

    Wounds LordSkreechVerminking::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Skaventide::applyWoundSave(wounds, attackingUnit);

        // Protection of the Horned Rat
        return ignoreWounds(totalWounds, 5);
    }

    int LordSkreechVerminking::terrifying(const Unit *target) {
        // Terrifying
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    void LordSkreechVerminking::onWounded() {
        Unit::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_plaguereaper.setAttacks(g_damageTable[damageIndex].m_reaperAttacks);
    }

    void LordSkreechVerminking::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t LordSkreechVerminking::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int LordSkreechVerminking::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
