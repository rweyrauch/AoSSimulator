/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include <climits>
#include <Roster.h>
#include "slavestodarkness/VortexBeast.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 170;

    struct TableEntry {
        int m_move;
        int m_clawToWound;
        int m_mawAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 10, 13, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 1, RAND_3D6},
                    {8,  2, RAND_2D6},
                    {8,  3, RAND_D6},
                    {6,  4, RAND_D3},
                    {4,  5, 1}
            };

    bool MutalithVortexBeast::s_registered = false;

    Unit *MutalithVortexBeast::Create(const ParameterList &parameters) {
        auto unit = new MutalithVortexBeast();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int MutalithVortexBeast::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void MutalithVortexBeast::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MutalithVortexBeast::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    MutalithVortexBeast::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, TZEENTCH}
            };
            s_registered = UnitFactory::Register("Mutalith Vortex Beast", factoryMethod);
        }
    }

    MutalithVortexBeast::MutalithVortexBeast() :
            SlavesToDarknessBase("Mutalith Vortex Beast", 10, g_wounds, 7, 4, false),
            m_claws(Weapon::Type::Melee, "Crushing Claws", 2, 4, 4, 1, -1, RAND_D3),
            m_maw(Weapon::Type::Melee, "Betentacled Maw", 2, RAND_3D6, 4, 4, 0, 1) {
        m_keywords = {CHAOS, SLAVES_TO_DARKNESS, MONSTER, MUTALITH_VORTEX_BEAST};
        m_weapons = {&m_claws, &m_maw};
        m_battleFieldRole = Role::Behemoth;
    }

    bool MutalithVortexBeast::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_maw);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    void MutalithVortexBeast::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            // Mutant Regeneration
            heal(Dice::RollD3());

            // Aura of Mutilation
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 18.0);
            for (auto unit : units) {
                const auto roll = Dice::RollD6();
                switch (roll) {
                    case 1:
                        // Hideous Disfigurement
                        unit->buffModifier(Attribute::Bravery, -1, {Phase::Hero, INT_MAX, player});
                        break;
                    case 2:
                        // Troggbrains
                        unit->buffModifier(Attribute::Run_Distance, -1, {Phase::Hero, INT_MAX, player});
                        break;
                    case 3:
                        // Gift of Mutations
                        unit->buffModifier(Attribute::Move_Distance, -1, {Phase::Hero, INT_MAX, player});
                        break;
                    case 4:
                        // Tide of Transmogrification
                        unit->applyDamage({0, Dice::RollD3()}, this);
                        break;
                    case 5:
                        // Maelstrom of Change
                        unit->applyDamage({0, Dice::RollD6()}, this);
                        break;
                    case 6: {
                        // Spawnchange
                        auto numSlain = unit->applyDamage({0, Dice::RollD6()}, this);
                        if (numSlain) {
                            if (remainingWounds() < initialWounds()) {
                                heal(Dice::RollD3());
                            } else {
                                // Summon a Chaos Spawn
                                if (m_roster) {
                                    auto factory = UnitFactory::LookupUnit("Chaos Spawn");
                                    if (factory) {
                                        auto spawn = std::shared_ptr<Unit>(
                                                UnitFactory::Create("Chaos Spawn", factory->m_parameters));
                                        spawn->deploy(position(), m_orientation);
                                        m_roster->addUnit(spawn);
                                    }
                                }

                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
                // Only apply to first enemy unit found
                break;
            }
        }
    }

    void MutalithVortexBeast::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_claws.setToWound(g_damageTable[damageIndex].m_clawToWound);
        m_maw.setAttacks(g_damageTable[damageIndex].m_mawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void MutalithVortexBeast::onRestore() {
        SlavesToDarknessBase::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    int MutalithVortexBeast::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

} // namespace SlavesToDarkness


