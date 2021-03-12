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
#include <Board.h>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 100;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 390;

    struct TableEntry {
        int m_move;
        int m_heartrenderAttacks;
        int m_tailDamage;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 8, 10, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 8, 6},
                    {12, 7, 5},
                    {10, 6, 4},
                    {8,  5, 3},
                    {6,  2, 2}
            };

    bool MorathiTheShadowQueen::s_registered = false;

    MorathiTheShadowQueen::MorathiTheShadowQueen() :
            DaughterOfKhaine("The Shadow Queen", 14, g_wounds, 9, 4, true),
            m_gaze(Weapon::Type::Missile, "Gaze of the Shadow Queen", 18, 1, 2, 2, -3, RAND_D6),
            m_heartrender(Weapon::Type::Melee, "Heartrender", 2, 8, 3, 3, -2, 3),
            m_crown(Weapon::Type::Melee, "Crown of Serpents", 1, RAND_2D6, 3, 3, 0, 1),
            m_tail(Weapon::Type::Melee, "Envenomed Tail", 3, 1, 3, 3, -2, 6) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, MONSTER, HERO, THE_SHADOW_QUEEN};
        m_weapons = {&m_gaze, &m_heartrender, &m_crown, &m_tail};
        m_battleFieldRole = Role::Leader;

        s_globalAttackMod.connect(this, &MorathiTheShadowQueen::furyOfTheShadowQueen, &m_furySlot);
    }

    MorathiTheShadowQueen::~MorathiTheShadowQueen() {
        m_furySlot.disconnect();
    }

    bool MorathiTheShadowQueen::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_gaze);
        model->addMeleeWeapon(&m_heartrender);
        model->addMeleeWeapon(&m_crown);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *MorathiTheShadowQueen::Create(const ParameterList &parameters) {
        auto unit = new MorathiTheShadowQueen();

        auto temple = (Temple)GetEnumParam("Temple", parameters, g_temple[0]);
        unit->setTemple(temple);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
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

            s_registered = UnitFactory::Register("The Shadow Queen", factoryMethod);
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
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int MorathiTheShadowQueen::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int MorathiTheShadowQueen::furyOfTheShadowQueen(const Unit *attacker, const Model *attackingModel,
                                                    const Weapon *weapon, const Unit *target) {
        auto mod = 0;
        if (!weapon->isMissile() && isFriendly(attacker) && (attacker->hasKeyword(KHINERAI_HARPIES) || attacker->hasKeyword(MELUSAI))) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0f) && (distanceTo(attacker) <= 18.0f)) {
                mod++;
            }
        }
        return mod;
    }


} //namespace DaughtersOfKhaine

