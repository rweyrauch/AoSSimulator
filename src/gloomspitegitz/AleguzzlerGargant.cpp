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

namespace GloomspiteGitz {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 160;

    struct TableEntry {
        int m_move;
        int m_clubAttacks;
        int m_eadbuttDamage;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {8, RAND_3D6, RAND_D6},
                    {6, RAND_2D6, RAND_D6},
                    {5, RAND_2D6, RAND_D3},
                    {4, RAND_D6,  RAND_D3},
                    {3, RAND_D6,  1}
            };

    bool AleguzzlerGargant::s_registered = false;

    AleguzzlerGargant::AleguzzlerGargant() :
            GloomspiteGitzBase("Aleguzzler Gargant", 8, WOUNDS, 6, 5, false),
            m_eadbutt(Weapon::Type::Melee, "'eadbutt", 1, 1, 4, 3, -3, RAND_D6),
            m_massiveClub(Weapon::Type::Melee, "Massive Club", 3, RAND_3D6, 3, 3, -1, 1),
            m_mightyKick(Weapon::Type::Melee, "Mighty Kick", 3, 1, 3, 3, -2, RAND_D3) {
        m_keywords = {DESTRUCTION, GARGANT, GLOOMSPITE_GITZ, ALEGUZZLER, MONSTER};
        m_weapons = {&m_eadbutt, &m_massiveClub, &m_mightyKick};
    }

    bool AleguzzlerGargant::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_eadbutt);
        model->addMeleeWeapon(&m_massiveClub);
        model->addMeleeWeapon(&m_mightyKick);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void AleguzzlerGargant::onWounded() {
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_eadbutt.setDamage(g_damageTable[getDamageTableIndex()].m_eadbuttDamage);
        m_massiveClub.setAttacks(g_damageTable[getDamageTableIndex()].m_clubAttacks);
    }

    Unit *AleguzzlerGargant::Create(const ParameterList &parameters) {
        auto unit = new AleguzzlerGargant();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AleguzzlerGargant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AleguzzlerGargant::Create,
                    nullptr,
                    nullptr,
                    AleguzzlerGargant::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Aleguzzler Gargant", factoryMethod);
        }
    }

    int AleguzzlerGargant::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void AleguzzlerGargant::onRestore() {
        Unit::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    int AleguzzlerGargant::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void AleguzzlerGargant::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Stuff'Em In Me Bag
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 3.0f)) {
            if (Dice::rollD6() >= unit->wounds()*2) {
                unit->slay(1);
            }
        }
    }

} // namespace GloomspiteGitz
