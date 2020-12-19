/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRunefatherOnMagmadroth.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 270;

    struct TableEntry {
        int m_move;
        int m_roaringFyrestream;
        int m_clawsHornsAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, RAND_D6,  6},
                    {10, RAND_D6,  5},
                    {8,  RAND_2D6, 4},
                    {7,  RAND_2D6, 3},
                    {6,  RAND_3D6, 2}
            };

    bool AuricRunefatherOnMagmadroth::s_registered = false;

    AuricRunefatherOnMagmadroth::AuricRunefatherOnMagmadroth() :
            Fyreslayer("Auric Runefather on Magmadroth", 12, g_wounds, 8, 4, false),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_fyrestream(Weapon::Type::Missile, "", 12, 1, 0, 0, 0, 0),
            m_clawsAndHorns(Weapon::Type::Melee, "Claws and Horns", 1, 6, 4, 3, -1, 2),
            m_blazingMaw(Weapon::Type::Melee, "Blazing Maw", 1, 1, 4, 2, -2, RAND_D3),
            m_grandAxe(Weapon::Type::Melee, "Latchkey Grandaxe", 3, 3, 3, 3, -1, 3) {
        m_keywords = {ORDER, DUARDIN, MAGMADROTH, FYRESLAYERS, MONSTER, HERO, AURIC_RUNEFATHER};
        m_weapons = {&m_throwingAxe, &m_fyrestream, &m_clawsAndHorns, &m_blazingMaw, &m_grandAxe};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;
    }

    bool AuricRunefatherOnMagmadroth::configure(MountTrait trait) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMissileWeapon(&m_fyrestream);
        model->addMeleeWeapon(&m_clawsAndHorns);
        model->addMeleeWeapon(&m_blazingMaw);
        model->addMeleeWeapon(&m_grandAxe);
        addModel(model);

        m_mountTrait = trait;

        m_points = g_pointsPerUnit;

        return true;
    }

    void AuricRunefatherOnMagmadroth::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Unit *AuricRunefatherOnMagmadroth::Create(const ParameterList &parameters) {
        auto unit = new AuricRunefatherOnMagmadroth();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_fatherSonTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_heirloomArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto mount = (MountTrait) GetEnumParam("Mount Trait", parameters, g_mountTraits[0]);

        bool ok = unit->configure(mount);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuricRunefatherOnMagmadroth::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRunefatherOnMagmadroth::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRunefatherOnMagmadroth::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Mount Trait", g_mountTraits[0], g_mountTraits),
                            EnumParameter("Command Trait", g_fatherSonTraits[0], g_fatherSonTraits),
                            EnumParameter("Artefact", g_heirloomArtefacts[0], g_heirloomArtefacts),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runefather on Magmadroth", factoryMethod);
        }
    }

    void AuricRunefatherOnMagmadroth::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_clawsAndHorns.setAttacks(g_damageTable[damageIndex].m_clawsHornsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int AuricRunefatherOnMagmadroth::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void AuricRunefatherOnMagmadroth::onStartShooting(PlayerId player) {
        Fyreslayer::onStartShooting(player);
        if (player == owningPlayer()) {
            // Roaring Fyrestream
            if (m_shootingTarget) {
                double dist = distanceTo(m_shootingTarget);
                if (dist <= (double) m_fyrestream.range()) {
                    int rs = Dice::RollSpecial(g_damageTable[getDamageTableIndex()].m_roaringFyrestream);
                    if (rs <= m_shootingTarget->remainingModels()) {
                        if (dist < 6.0) {
                            m_shootingTarget->applyDamage({0, Dice::RollD6()}, this);
                        } else {
                            m_shootingTarget->applyDamage({0, Dice::RollD3()}, this);
                        }
                    }
                }
            }
        }
    }


    Wounds AuricRunefatherOnMagmadroth::onEndCombat(PlayerId player) {
        auto wounds = Fyreslayer::onEndCombat(player);

        // Lashing Tail
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto ip : units) {
            if (Dice::RollD6() < ip->remainingModels()) {
                Wounds tailWounds = {0, Dice::RollD3()};
                ip->applyDamage(tailWounds, this);
                wounds += tailWounds;
            }
        }

        // Weapon-breaker
        auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
        if (unit) {
            if (Dice::RollD6() == 6) {
                // TODO: buff only affects a single weapon
                unit->buffModifier(To_Hit_Melee, -1, {Battleshock, std::numeric_limits<int>::max(), owningPlayer()});
            }
        }

        return wounds;
    }

    Wounds AuricRunefatherOnMagmadroth::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        if (!weapon->isMissile()) {
            // Volcanic Blood
            if (Dice::RollD6() >= 4) {
                return {0, 1};
            }
        }
        return Fyreslayer::computeReturnedDamage(weapon, saveRoll);
    }

    int AuricRunefatherOnMagmadroth::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void AuricRunefatherOnMagmadroth::onStartHero(PlayerId player) {
        Fyreslayer::onStartHero(player);

        if (owningPlayer() == player) {
            // Stare Down
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0)) {
                unit->buffModifier(Bravery, -Dice::RollD3(), {Hero, m_battleRound + 1, owningPlayer()});
            }
        }
    }

} // namespace Fyreslayers