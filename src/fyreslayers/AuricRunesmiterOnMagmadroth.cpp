/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunesmiterOnMagmadroth.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 250;

    struct TableEntry {
        int m_move;
        int m_roaringFyrestream;
        int m_clawsHornsAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, RAND_D6,  6},
                    {10, RAND_D6,  5},
                    {8,  RAND_2D6, 4},
                    {7,  RAND_2D6, 3},
                    {6,  RAND_3D6, 2}
            };

    bool AuricRunesmiterOnMagmadroth::s_registered = false;

    AuricRunesmiterOnMagmadroth::AuricRunesmiterOnMagmadroth() :
            Fyreslayer("Auric Runesmiter on Magmadroth", 12, g_wounds, 8, 4, false),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_fyrestream(Weapon::Type::Missile, "", 12, 1, 0, 0, 0, 0),
            m_clawsAndHorns(Weapon::Type::Melee, "Claws and Horns", 1, 6, 4, 3, -1, 2),
            m_blazingMaw(Weapon::Type::Melee, "Blazing Maw", 1, 1, 4, 2, -2, RAND_D3),
            m_latchAxe(Weapon::Type::Melee, "Latch-axe", 1, 1, 3, 3, 0, 2),
            m_runicIron(Weapon::Type::Melee, "Runic Iron", 1, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, MAGMADROTH, FYRESLAYERS, MONSTER, HERO, PRIEST, AURIC_RUNESMITER};
        m_weapons = {&m_throwingAxe, &m_fyrestream, &m_clawsAndHorns, &m_blazingMaw, &m_latchAxe, &m_runicIron};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;
    }

    bool AuricRunesmiterOnMagmadroth::configure(Prayer prayer, MountTrait trait) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMissileWeapon(&m_fyrestream);
        model->addMeleeWeapon(&m_clawsAndHorns);
        model->addMeleeWeapon(&m_blazingMaw);
        model->addMeleeWeapon(&m_latchAxe);
        model->addMeleeWeapon(&m_runicIron);
        addModel(model);

        m_mountTrait = trait;

        m_points = g_pointsPerUnit;

        return true;
    }

    void AuricRunesmiterOnMagmadroth::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Unit *AuricRunesmiterOnMagmadroth::Create(const ParameterList &parameters) {
        auto unit = new AuricRunesmiterOnMagmadroth();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterSmiterTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_forgeTempleArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto prayer = (Prayer) GetEnumParam("Prayer", parameters, g_prayers[0]);
        auto mount = (MountTrait) GetEnumParam("Mount Trait", parameters, g_mountTraits[0]);

        bool ok = unit->configure(prayer, mount);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuricRunesmiterOnMagmadroth::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRunesmiterOnMagmadroth::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRunesmiterOnMagmadroth::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Command Trait", g_masterSmiterTraits[0], g_masterSmiterTraits),
                            EnumParameter("Artefact", g_forgeTempleArtefacts[0], g_forgeTempleArtefacts),
                            EnumParameter("Mount Trait", g_mountTraits[0], g_mountTraits),
                            EnumParameter("Prayer", g_prayers[0], g_prayers),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runesmiter on Magmadroth", factoryMethod);
        }
    }

    void AuricRunesmiterOnMagmadroth::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_clawsAndHorns.setAttacks(g_damageTable[damageIndex].m_clawsHornsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int AuricRunesmiterOnMagmadroth::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void AuricRunesmiterOnMagmadroth::onStartShooting(PlayerId player) {
        Fyreslayer::onStartShooting(player);
        if (player == owningPlayer()) {
            // Roaring Fyrestream
            if (m_shootingTarget) {
                double dist = distanceTo(m_shootingTarget);
                if (dist <= (double) m_fyrestream.range()) {
                    int rs = Dice::RollSpecial(g_damageTable[getDamageTableIndex()].m_roaringFyrestream);
                    if (rs <= m_shootingTarget->remainingModels()) {
                        if (dist < 6.0) {
                            m_shootingTarget->applyDamage({0, Dice::RollD6()});
                        } else {
                            m_shootingTarget->applyDamage({0, Dice::RollD3()});
                        }
                    }
                }
            }
        }
    }


    Wounds AuricRunesmiterOnMagmadroth::onEndCombat(PlayerId player) {
        auto wounds = Fyreslayer::onEndCombat(player);

        // Lashing Tail
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto ip : units) {
            if (Dice::RollD6() < ip->remainingModels()) {
                Wounds tailWounds = {0, Dice::RollD3()};
                ip->applyDamage(tailWounds);
                wounds += tailWounds;
            }
        }
        return wounds;
    }

    Wounds AuricRunesmiterOnMagmadroth::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        if (!weapon->isMissile()) {
            // Volcanic Blood
            if (Dice::RollD6() >= 4) {
                return {0, 1};
            }
        }
        return Fyreslayer::computeReturnedDamage(weapon, saveRoll);
    }

    int AuricRunesmiterOnMagmadroth::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Fyreslayers