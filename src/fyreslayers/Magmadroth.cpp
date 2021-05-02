/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/Magmadroth.h>
#include <Board.h>

namespace Fyreslayers {
    static const int g_wounds = 14;

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

    Magmadroth::Magmadroth(const std::string &name, int bravery, int points) :
            Fyreslayer(name, 12, g_wounds, bravery, 4, false, points),
            m_fyrestream(Weapon::Type::Missile, "", 12, 1, 0, 0, 0, 0),
            m_clawsAndHorns(Weapon::Type::Melee, "Claws and Horns", 1, 6, 4, 3, -1, 2),
            m_blazingMaw(Weapon::Type::Melee, "Blazing Maw", 1, 1, 4, 2, -2, RAND_D3) {
        m_hasMount = true;
        m_fyrestream.setMount(true);
        m_clawsAndHorns.setMount(true);
        m_blazingMaw.setMount(true);
    }

    void Magmadroth::onRestore() {
        Fyreslayer::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    void Magmadroth::onWounded() {
        Fyreslayer::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_clawsAndHorns.setAttacks(g_damageTable[damageIndex].m_clawsHornsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void Magmadroth::onStartShooting(PlayerId player) {
        Fyreslayer::onStartShooting(player);
        if (player == owningPlayer()) {
            // Roaring Fyrestream
            if (m_shootingTarget) {
                double dist = distanceTo(m_shootingTarget);
                if (dist <= (double) m_fyrestream.range()) {
                    int rs = Dice::RollSpecial(g_damageTable[getDamageTableIndex()].m_roaringFyrestream);
                    if (m_mountTrait == MountTrait::Lava_Tongue_Adult) {
                        rs--;
                        // TODO: also check for w/in 6" of an objective
                        if (Board::Instance()->isUnitWithinDeploymentZone(this, owningPlayer())) {
                            rs--;
                        }
                    }
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

    void Magmadroth::onEndCombat(PlayerId player) {
        Fyreslayer::onEndCombat(player);

        // Lashing Tail
        int rollMod = 0;
        if (m_mountTrait == MountTrait::Cinder_Crest_Youngblood) {
            rollMod = -1;
            if (charged()) rollMod = -2;
        }
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto ip : units) {
            if (Dice::RollD6() + rollMod < ip->remainingModels()) {
                Wounds tailWounds = {0, Dice::RollD3()};
                ip->applyDamage(tailWounds, this);
            }
        }
    }

    Wounds Magmadroth::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        if (!weapon->isMissile()) {
            // Volcanic Blood
            if (Dice::RollD6() >= 4) {
                return {0, 1};
            }
        }
        return Fyreslayer::computeReturnedDamage(weapon, saveRoll);
    }

    size_t Magmadroth::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Magmadroth::onCharged() {
        Fyreslayer::onCharged();

        if (m_mountTrait == MountTrait::Flame_Scale_Youngblood) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && distanceTo(unit) < 1.0) {
                Dice::RollResult rolls;
                Dice::RollD6(m_unmodifiedChargeRoll, rolls);
                unit->applyDamage({0, rolls.rollsGE(6), Wounds::Source::Ability}, this);
            }
        }
    }

    int Magmadroth::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = Fyreslayer::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);

        if ((m_mountTrait == MountTrait::Fire_Claw_Adult) && weapon->isMelee() && weapon->isMount() &&
            (woundRoll == 6)) {
            rend = -3;
        }

        return rend;
    }

    Rerolls Magmadroth::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        if (m_mountTrait == MountTrait::Ash_Horn_Ancient) {
            return Rerolls::Ones;
        }

        auto units = Board::Instance()->getUnitsWithKeyword(owningPlayer(), MAGMADROTH);
        for (auto unit : units) {
            auto droth = dynamic_cast<Magmadroth *>(unit);
            if (droth && (droth->m_mountTrait == MountTrait::Ash_Horn_Ancient) && distanceTo(droth) < 6.0) {
                return Rerolls::Ones;
            }
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

} // namespace Fyreslayers