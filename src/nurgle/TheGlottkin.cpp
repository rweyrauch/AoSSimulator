/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/TheGlottkin.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 130;
    static const int g_wounds = 18;
    static const int g_pointsPerUnit = 380;

    struct TableEntry {
        int m_move;
        int m_torrentDamage;
        int m_tentacleAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, RAND_2D6, 6},
                    {7, RAND_D6,  5},
                    {6, RAND_D3,  4},
                    {5, 2,        3},
                    {4, 1,        2}
            };

    bool TheGlottkin::s_registered = false;

    TheGlottkin::TheGlottkin() :
            NurgleBase("The Glottkin", 8, g_wounds, 9, 4, false),
            m_pestilentTorrent(Weapon::Type::Missile, "Pestilent Torrent", 12, 1, 3, 4, -2, RAND_2D6),
            m_flailingTentacle(Weapon::Type::Melee, "Ghurk's Flailing Tentacle", 3, 6, 4, 2, -2, 2),
            m_lampreyMaw(Weapon::Type::Melee, "Ghurk's Lamprey Maw", 2, 1, 3, 2, -1, RAND_D3),
            m_poisonScythe(Weapon::Type::Melee, "Otto's Poison-slick Scythe", 2, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, MONSTER, HERO, WIZARD, THE_GLOTTKIN};
        m_weapons = {&m_pestilentTorrent, &m_flailingTentacle, &m_lampreyMaw, &m_poisonScythe};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalUnbinds = 1;
        m_totalSpells = 2;
    }

    bool TheGlottkin::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_pestilentTorrent);
        model->addMeleeWeapon(&m_flailingTentacle);
        model->addMeleeWeapon(&m_lampreyMaw);
        model->addMeleeWeapon(&m_poisonScythe);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *TheGlottkin::Create(const ParameterList &parameters) {
        auto unit = new TheGlottkin();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        unit->setLegion(legion);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_mortalRotbringerLore[0]);

        unit->configure(lore);
        return unit;
    }

    void TheGlottkin::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheGlottkin::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    TheGlottkin::ComputePoints,
                    {
                            EnumParameter("Lore", g_mortalRotbringerLore[0], g_mortalRotbringerLore),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("The Glottkin", factoryMethod);
        }
    }

    void TheGlottkin::onRestore() {
        NurgleBase::onRestore();
        // Reset table-drive attributes
        onWounded();
    }

    void TheGlottkin::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                // Blessing of Nurgle
                int woundsHealed = Dice::RollD3();
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }
        }
        NurgleBase::onStartHero(player);
    }

    void TheGlottkin::onWounded() {
        NurgleBase::onWounded();
        const int damageIndex = getDamageTableIndex();
        m_pestilentTorrent.setDamage(g_damageTable[damageIndex].m_torrentDamage);
        m_flailingTentacle.setAttacks(g_damageTable[damageIndex].m_tentacleAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t TheGlottkin::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void TheGlottkin::onCharged() {
        // Mountain of Loathsome Flesh
        if (m_charged) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto ip : units) {
                int roll = Dice::RollD6();
                if (roll >= 4) {
                    ip->applyDamage({Dice::RollD3(), 0}, this);
                }
            }
        }
        NurgleBase::onCharged();
    }

    int TheGlottkin::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle

