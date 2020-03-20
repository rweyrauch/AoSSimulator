/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/HellPitAbomination.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Skaven {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 240;

    struct TableEntry {
        int m_teethRend;
        int m_fistsAttacks;
        int m_avalancheOfFlesh;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {-3, 6, 2},
                    {-2, 5, 3},
                    {-2, 4, 4},
                    {-1, 3, 5},
                    {-1, 2, 6}
            };

    bool HellPitAbomination::s_registered = false;

    HellPitAbomination::HellPitAbomination() :
            Skaventide("Hell Pit Abomination", RAND_2D6, WOUNDS, 6, 5, false),
            m_gnashingTeath(Weapon::Type::Melee, "Gnashing Teeth", 1, 6, 3, 3, -3, 2),
            m_flailingFists(Weapon::Type::Melee, "Flailing Fists", 2, 6, 3, 3, -1, 3),
            m_avalancheOfFlesh(Weapon::Type::Melee, "Avalanche of Flesh", 1, 0, 0, 0, 0, 0) {
        m_keywords = {CHAOS, SKAVENTIDE, CLANS_MOULDER, FIGHTING_BEAST, MONSTER, HELL_PIT_ABOMINATION};
        m_weapons = {&m_gnashingTeath, &m_flailingFists, &m_avalancheOfFlesh};

        s_globalBraveryMod.connect(this, &HellPitAbomination::terrifying, &m_connection);
    }

    HellPitAbomination::~HellPitAbomination() {
        m_connection.disconnect();
    }

    bool HellPitAbomination::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_gnashingTeath);
        model->addMeleeWeapon(&m_flailingFists);
        model->addMeleeWeapon(&m_avalancheOfFlesh);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int HellPitAbomination::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *HellPitAbomination::Create(const ParameterList &parameters) {
        auto unit = new HellPitAbomination();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void HellPitAbomination::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    nullptr,
                    nullptr,
                    ComputePoints,
                    {
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Hell Pit Abomination", factoryMethod);
        }
    }

    int HellPitAbomination::generateMortalWounds(const Unit *unit) {
        int mortalWounds = 0;

        // Avalanche of Flesh
        if (distanceTo(unit) <= 3.0f) {
            // TODO: check distance for each model in target unit
            int numRolls = unit->remainingModels();
            for (auto i = 0; i < numRolls; i++) {
                int roll = Dice::rollD6();
                if (roll >= g_damageTable[getDamageTableIndex()].m_avalancheOfFlesh) {
                    mortalWounds++;
                } else if (m_charged) // re-roll if charged
                {
                    roll = Dice::rollD6();
                    if (roll >= g_damageTable[getDamageTableIndex()].m_avalancheOfFlesh) {
                        mortalWounds++;
                    }
                }
            }
        }

        mortalWounds += Skaventide::generateMortalWounds(unit);

        return mortalWounds;
    }

    void HellPitAbomination::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            // Regenerating Monstrosity
            if (remainingWounds() < WOUNDS && remainingWounds() > 0) {
                int woundsHealed = Dice::rollD3();
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }
        }
    }

    void HellPitAbomination::onSlain() {
        Skaventide::onSlain();

        // Too Horrible to Die
        if (!m_beenSlain) {
            int roll = Dice::rollD6();

            if (roll >= 5) {
                // It's Alive!
                roll = Dice::rollD6();
                m_models.front()->restore();
                m_models.front()->applyWound(WOUNDS - roll);
            } else if (roll >= 3) {
                // The Rats Emerge
                auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0f);
                for (auto ip : units) {
                    roll = Dice::rollD3();
                    ip->applyDamage({0, roll});
                }
            }
            m_beenSlain = true;
        }
    }

    int HellPitAbomination::terrifying(const Unit *unit) {
        // Terrifying
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 3.0f)) {
            return -1;
        }
        return 0;
    }

    int HellPitAbomination::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} //namespace Skaven
