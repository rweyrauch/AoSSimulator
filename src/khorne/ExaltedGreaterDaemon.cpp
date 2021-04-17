/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/ExaltedGreaterDaemon.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 20;
    static const int g_pointsPerUnit = 540;

    struct TableEntry {
        int m_move;
        int m_lashAttacks;
        int m_axeToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {4, 8, 12, 16, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 6, 2},
                    {10,  5, 2},
                    {9,  4, 3},
                    {8,  3, 3},
                    {7,  2, 4}
            };

    bool ExaltedGreaterDaemonOfKhorne::s_registered = false;

    ExaltedGreaterDaemonOfKhorne::ExaltedGreaterDaemonOfKhorne(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Exalted Greater Daemon of Khorne", 12, g_wounds, 10, 4, true) {
        m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, EXALTED_GREATER_DAEMON_OF_KHORNE};
        m_weapons = {&m_lashOfKhorne, &m_mightyAxeOfKhorne};
        m_battleFieldRole = Role::Leader_Behemoth;

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_lashOfKhorne);
        model->addMeleeWeapon(&m_mightyAxeOfKhorne);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffModifierCommandAbility>(this, "Rejoice in Exalted Slaughter", 18, 18, GamePhase::Combat,
                                                             Attribute::Pile_In_Distance, 3,
                                                             Abilities::Target::SelfAndFriendly,
                                                             std::vector<Keyword>{KHORNE, DAEMON}));

        m_points = g_pointsPerUnit;
    }

    Unit *ExaltedGreaterDaemonOfKhorne::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new ExaltedGreaterDaemonOfKhorne(host, trait, artefact, general);
    }

    void ExaltedGreaterDaemonOfKhorne::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ExaltedGreaterDaemonOfKhorne::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    ExaltedGreaterDaemonOfKhorne::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Exalted Greater Daemon of Khorne", factoryMethod);
        }
    }

    size_t ExaltedGreaterDaemonOfKhorne::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void ExaltedGreaterDaemonOfKhorne::onRestore() {
        // Restore table-driven attributes
        onWounded();
        KhorneBase::onRestore();
    }

    void ExaltedGreaterDaemonOfKhorne::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_lashOfKhorne.setAttacks(g_damageTable[damageIndex].m_lashAttacks);
        m_mightyAxeOfKhorne.setToWound(g_damageTable[damageIndex].m_axeToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        KhorneBase::onWounded();
    }

    void ExaltedGreaterDaemonOfKhorne::onStartHero(PlayerId player) {
        KhorneBase::onStartHero(player);
        // The Land Rebels
        auto enemyUnits = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 8.0);
        for (auto ip : enemyUnits) {
            if (Dice::RollD6() >= 5) {
                ip->applyDamage({0, 1}, this);
            }
        }
    }

    void ExaltedGreaterDaemonOfKhorne::onStartMovement(PlayerId player) {
        KhorneBase::onStartMovement(player);

        // Drawn in for the Kill
        if (GetEnemyId(owningPlayer()) == player) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) < 3.0)) {
                unit->buffMovement(MovementRule::Can_Retreat, false, {GamePhase::Movement, m_battleRound, player});
            }
        }
    }

    int ExaltedGreaterDaemonOfKhorne::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne