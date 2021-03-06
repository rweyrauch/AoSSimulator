/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/AleguzzlerGargant.h>
#include <UnitFactory.h>
#include <Board.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 160;

    struct TableEntry {
        int m_move;
        int m_clubAttacks;
        int m_eadbuttDamage;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, RAND_3D6, RAND_D6},
                    {6, RAND_2D6, RAND_D6},
                    {5, RAND_2D6, RAND_D3},
                    {4, RAND_D6,  RAND_D3},
                    {3, RAND_D6,  1}
            };

    bool AleguzzlerGargant::s_registered = false;

    AleguzzlerGargant::AleguzzlerGargant(Allegiance allegiance) :
            GloomspiteGitzBase(allegiance, "Aleguzzler Gargant", 8, g_wounds, 6, 5, false, g_pointsPerUnit),
            m_eadbutt(Weapon::Type::Melee, "'eadbutt", 1, 1, 4, 3, -3, RAND_D6),
            m_massiveClub(Weapon::Type::Melee, "Massive Club", 3, RAND_3D6, 3, 3, -1, 1),
            m_mightyKick(Weapon::Type::Melee, "Mighty Kick", 3, 1, 3, 3, -2, RAND_D3) {
        m_keywords = {DESTRUCTION, GARGANT, GLOOMSPITE_GITZ, ALEGUZZLER, MONSTER};
        m_weapons = {&m_eadbutt, &m_massiveClub, &m_mightyKick};
        m_battleFieldRole = Role::Behemoth;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_eadbutt);
        model->addMeleeWeapon(&m_massiveClub);
        model->addMeleeWeapon(&m_mightyKick);
        addModel(model);
    }

    void AleguzzlerGargant::onWounded() {
        GloomspiteGitzBase::onWounded();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_eadbutt.setDamage(g_damageTable[getDamageTableIndex()].m_eadbuttDamage);
        m_massiveClub.setAttacks(g_damageTable[getDamageTableIndex()].m_clubAttacks);
    }

    Unit *AleguzzlerGargant::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        return new AleguzzlerGargant(allegiance);
    }

    void AleguzzlerGargant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AleguzzlerGargant::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    AleguzzlerGargant::ComputePoints,
                    {
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Aleguzzler Gargant", factoryMethod);
        }
    }

    size_t AleguzzlerGargant::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void AleguzzlerGargant::onRestore() {
        GloomspiteGitzBase::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    int AleguzzlerGargant::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void AleguzzlerGargant::onStartCombat(PlayerId player) {
        GloomspiteGitzBase::onStartCombat(player);

        // Stuff'Em In Me Bag
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 3.0)) {
            if (Dice::RollD6() >= unit->wounds() * 2) {
                unit->slay(1);
            }
        }
    }

} // namespace GloomspiteGitz
