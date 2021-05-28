/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/StarshardBallista.h>
#include <UnitFactory.h>
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    static const int g_basesize = 80;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool VanariStarshardBallistas::s_registered = false;

    Unit *VanariStarshardBallistas::Create(const ParameterList &parameters) {
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        return new VanariStarshardBallistas(nation);
    }

    int VanariStarshardBallistas::ComputePoints(const ParameterList& parameters) {
        return g_pointsPerUnit;
    }

    void VanariStarshardBallistas::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };
            s_registered = UnitFactory::Register("Vanari Starshard Ballistas", factoryMethod);
        }
    }

    VanariStarshardBallistas::VanariStarshardBallistas(GreatNation nation) :
            LuminethBase(nation, "Vanari Starshard Ballistas", 6, g_wounds, 6, 5, false, g_pointsPerUnit) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, STARSHARD_BALLISTAS};
        m_weapons = {&m_bolts, &m_swords};
        m_battleFieldRole = Role::Artillery;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bolts);
        model->addMeleeWeapon(&m_swords);
        addModel(model);
    }

    int VanariStarshardBallistas::extraAttacks(const Model *attackingModel, const Weapon *weapon,
                                               const Unit *target) const {
        auto attacks = LuminethBase::extraAttacks(attackingModel, weapon, target);
        // Warding Lanterns
        if (!m_moved && (weapon->name() == m_bolts.name())) {
            PLOG_INFO << name() << " using Warding Lanterns to gain an additional attack.";
            attacks++;
        }
        return attacks;
    }

    Wounds VanariStarshardBallistas::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = LuminethBase::applyWoundSave(wounds, attackingUnit);
        // Warding Lanterns
        if (!m_moved) {
            int numSixes = 0;
            totalWounds = ignoreWounds(totalWounds, 6, numSixes);
            PLOG_INFO << name() << " using Warding Lanterns to ignore wounds." << wounds;
        }
        return totalWounds;
    }

    void VanariStarshardBallistas::onEndShooting(PlayerId player) {
        LuminethBase::onEndShooting(player);

        if ((owningPlayer() == player) && m_activatedBlindingBolts) {
            if (shootingTarget() && (m_currentRecord.m_attacksHitting > 0)) {
                PLOG_INFO << name() << " hit with Blinding Bolts against " << shootingTarget()->name();
                shootingTarget()->buffModifier(Attribute::To_Hit_Missile, -1, {GamePhase::Shooting, m_battleRound, owningPlayer()});
                shootingTarget()->buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Shooting, m_battleRound, owningPlayer()});

                m_activatedBlindingBolts = false;
            }
        }
    }

    void VanariStarshardBallistas::onStartShooting(PlayerId player) {
        LuminethBase::onStartShooting(player);

        if ((owningPlayer() == player) && !m_usedBlindingBolts) {
            if (shootingTarget() && (distanceTo(shootingTarget()) < m_bolts.range())) {
                m_activatedBlindingBolts = true;
                m_usedBlindingBolts = true;
                PLOG_INFO << name() << " activated Blinding Bolts against " << shootingTarget()->name();
            }
        }

        // Messenger Hawk
        if (owningPlayer() == player) {
            if (shootingTarget()) {
                auto heros = Board::Instance()->getUnitsWithin(shootingTarget(), owningPlayer(), 24.0);
                for (auto hero : heros) {
                    if (hero->hasKeyword(HERO) && (hero->remainingModels() > 0) && (distanceTo(hero) < 24.0)) {
                        PLOG_INFO << name() << " using Messenger Hawk against " << shootingTarget()->name();
                        buffModifier(Attribute::To_Hit_Missile, 1, {GamePhase::Shooting, m_battleRound, owningPlayer()});
                        break;
                    }
                }
            }
        }
    }
}