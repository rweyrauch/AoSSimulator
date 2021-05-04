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
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 220;

    struct TableEntry {
        int m_teethRend;
        int m_fistsAttacks;
        int m_avalancheOfFlesh;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {-3, 6, 2},
                    {-2, 5, 3},
                    {-2, 4, 4},
                    {-1, 3, 5},
                    {-1, 2, 6}
            };

    bool HellPitAbomination::s_registered = false;

    HellPitAbomination::HellPitAbomination() :
            Skaventide("Hell Pit Abomination", RAND_2D6, g_wounds, 6, 5, false, g_pointsPerUnit),
            m_gnashingTeath(Weapon::Type::Melee, "Gnashing Teeth", 1, 6, 3, 3, -3, 2),
            m_flailingFists(Weapon::Type::Melee, "Flailing Fists", 2, 6, 3, 3, -1, 3),
            m_avalancheOfFlesh(Weapon::Type::Melee, "Avalanche of Flesh", 1, 0, 0, 0, 0, 0) {
        m_keywords = {CHAOS, SKAVENTIDE, CLANS_MOULDER, FIGHTING_BEAST, MONSTER, HELL_PIT_ABOMINATION};
        m_weapons = {&m_gnashingTeath, &m_flailingFists, &m_avalancheOfFlesh};
        m_battleFieldRole = Role::Behemoth;

        s_globalBraveryMod.connect(this, &HellPitAbomination::terrifying, &m_connection);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_gnashingTeath);
        model->addMeleeWeapon(&m_flailingFists);
        model->addMeleeWeapon(&m_avalancheOfFlesh);
        addModel(model);
    }

    HellPitAbomination::~HellPitAbomination() {
        m_connection.disconnect();
    }

    size_t HellPitAbomination::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *HellPitAbomination::Create(const ParameterList &parameters) {
        return new HellPitAbomination();
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
        if (distanceTo(unit) <= 3.0) {
            // TODO: check distance for each model in target unit
            int numRolls = unit->remainingModels();
            for (auto i = 0; i < numRolls; i++) {
                int roll = Dice::RollD6();
                if (roll >= g_damageTable[getDamageTableIndex()].m_avalancheOfFlesh) {
                    mortalWounds++;
                } else if (m_charged) // re-roll if charged
                {
                    roll = Dice::RollD6();
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
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                int woundsHealed = Dice::RollD3();
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }
        }
    }

    void HellPitAbomination::onFriendlyUnitSlain(const Unit *attacker) {
        Skaventide::onFriendlyUnitSlain(nullptr);

        // Too Horrible to Die
        if (!m_beenSlain) {
            int roll = Dice::RollD6();

            if (roll >= 5) {
                // It's Alive!
                roll = Dice::RollD6();
                m_models.front()->restore();
                m_models.front()->applyWound(g_wounds - roll);
            } else if (roll >= 3) {
                // The Rats Emerge
                auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0);
                for (auto ip : units) {
                    roll = Dice::RollD3();
                    ip->applyDamage({0, roll}, this);
                }
            }
            m_beenSlain = true;
        }
    }

    int HellPitAbomination::terrifying(const Unit *unit) {
        // Terrifying
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    int HellPitAbomination::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
