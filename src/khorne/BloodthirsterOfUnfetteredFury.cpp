/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/BloodthirsterOfUnfetteredFury.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 270;

    struct TableEntry {
        int m_move;
        int m_lashAttacks;
        int m_axeToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 4, 2},
                    {9,  3, 3},
                    {8,  3, 3},
                    {7,  2, 4},
                    {6,  1, 4}
            };

    bool BloodthirsterOfUnfetteredFury::s_registered = false;

    BloodthirsterOfUnfetteredFury::BloodthirsterOfUnfetteredFury() :
            KhorneBase("Bloodthirster Of Unfettered Fury", 10, g_wounds, 10, 4, true),
            m_lashOfKhorne(Weapon::Type::Missile, "Lash of Khorne", 8, 4, 4, 3, -1, RAND_D3),
            m_mightyAxeOfKhorne(Weapon::Type::Melee, "Mighty Axe of Khorne", 2, 6, 3, 2, -2, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, BLOODTHIRSTER, KHORNE, MONSTER, HERO, BLOODTHIRSTER_OF_UNFETTERED_FURY};
        m_weapons = {&m_lashOfKhorne, &m_mightyAxeOfKhorne};
        m_battleFieldRole = Role::Leader_Behemoth;
    }

    bool BloodthirsterOfUnfetteredFury::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_lashOfKhorne);
        model->addMeleeWeapon(&m_mightyAxeOfKhorne);
        addModel(model);

        m_commandAbilities.push_back(
                std::make_unique<BuffModifierCommandAbility>(this, "Rejoice in the Slaughter", 16, 16, Phase::Combat,
                                                             Attribute::Pile_In_Distance, 3,
                                                             Abilities::Target::SelfAndFriendly,
                                                             std::vector<Keyword>{KHORNE, DAEMON}));
        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *BloodthirsterOfUnfetteredFury::Create(const ParameterList &parameters) {
        auto unit = new BloodthirsterOfUnfetteredFury();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BloodthirsterOfUnfetteredFury::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BloodthirsterOfUnfetteredFury::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    BloodthirsterOfUnfetteredFury::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodthirster Of Unfettered Fury", factoryMethod);
        }
    }

    int BloodthirsterOfUnfetteredFury::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void BloodthirsterOfUnfetteredFury::onRestore() {
        // Restore table-driven attributes
        onWounded();
        KhorneBase::onRestore();
    }

    void BloodthirsterOfUnfetteredFury::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_lashOfKhorne.setAttacks(g_damageTable[damageIndex].m_lashAttacks);
        m_mightyAxeOfKhorne.setToWound(g_damageTable[damageIndex].m_axeToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        KhorneBase::onWounded();
    }

    void BloodthirsterOfUnfetteredFury::onStartHero(PlayerId player) {
        KhorneBase::onStartHero(player);
        // The Land Rebels
        auto enemyUnits = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 8.0);
        for (auto ip : enemyUnits) {
            int roll = Dice::RollD6();
            if (roll == 6) {
                ip->applyDamage({0, 1}, this);
                // TODO: also movement of affected unit is halved until your next hero phase
            }
        }
    }

    int BloodthirsterOfUnfetteredFury::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void BloodthirsterOfUnfetteredFury::onStartMovement(PlayerId player) {
        KhorneBase::onStartMovement(player);

        // Drawn in for the Kill
        if (GetEnemyId(owningPlayer()) == player) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) < 3.0)) {
                unit->buffMovement(MovementRule::Can_Retreat, false, {Phase::Movement, m_battleRound, player});
            }
        }
    }

} // namespace Khorne