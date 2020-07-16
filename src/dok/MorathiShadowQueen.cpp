/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/MorathiShadowQueen.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int BASESIZE = 100;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 480;

    struct TableEntry {
        int m_move;
        int m_heartrenderAttacks;
        int m_tailDamage;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, 6, 6},
                    {12, 5, RAND_D6},
                    {10, 4, RAND_D6},
                    {8,  3, RAND_D3},
                    {6,  2, RAND_D3}
            };

    bool MorathiTheShadowQueen::s_registered = false;

    MorathiTheShadowQueen::MorathiTheShadowQueen() :
            DaughterOfKhaine("Morathi, The Shadow Queen", 14, WOUNDS, 9, 4, true),
            m_gaze(Weapon::Type::Missile, "Gaze of Morathi", 6, 1, 2, 0, 0, 0),
            m_heartrender(Weapon::Type::Melee, "Heartrender", 2, 6, 3, 3, -2, 3),
            m_crown(Weapon::Type::Melee, "Crown of Serpents", 1, 5, 3, 3, 0, 1),
            m_tail(Weapon::Type::Melee, "Envenomed Tail", 3, 1, 3, 3, -2, 6) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, MONSTER, HERO, WIZARD, MORATHI, THE_SHADOW_QUEEN};
        m_weapons = {&m_gaze, &m_heartrender, &m_crown, &m_tail};
        m_battleFieldRole = Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool MorathiTheShadowQueen::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_gaze);
        model->addMeleeWeapon(&m_heartrender);
        model->addMeleeWeapon(&m_crown);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *MorathiTheShadowQueen::Create(const ParameterList &parameters) {
        auto unit = new MorathiTheShadowQueen();

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MorathiTheShadowQueen::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Temple", g_temple[0], g_temple),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };

            s_registered = UnitFactory::Register("Morathi, The Shadow Queen", factoryMethod);
        }
    }

    void MorathiTheShadowQueen::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_heartrender.setAttacks(g_damageTable[damageIndex].m_heartrenderAttacks);
        m_tail.setDamage(g_damageTable[damageIndex].m_tailDamage);
        m_move = g_damageTable[damageIndex].m_move;
    }

    void MorathiTheShadowQueen::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int MorathiTheShadowQueen::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int MorathiTheShadowQueen::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace DaughtersOfKhaine

