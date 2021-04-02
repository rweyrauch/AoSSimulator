/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <stormcast/CelestarBallista.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {

    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 110;

    bool CelestarBallista::s_registered = false;

    CelestarBallista::CelestarBallista(Stormhost stormhost) :
            StormcastEternal(stormhost, "Celestar Ballista", 3, g_wounds, 7, 4, false) {

        // Burst of Celestial Energy
        m_stormboltsSingle.setHitsPerAttack(RAND_D6);
        m_stormboltsRapid.setHitsPerAttack(RAND_D6);

        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, ORDINATOS, WAR_MACHINE,
                      CELESTAR_BALLISTA};
        m_weapons = {&m_stormboltsSingle, &m_stormboltsRapid, &m_sigmariteBlades};
        m_battleFieldRole = Role::Artillery;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sigmariteBlades);

        m_stormboltsRapid.activate(false);
        m_stormboltsSingle.activate(true);

        model->addMissileWeapon(&m_stormboltsSingle);
        model->addMissileWeapon(&m_stormboltsRapid);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *CelestarBallista::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        return new CelestarBallista(stormhost);
    }

    void CelestarBallista::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Celestar Ballista", factoryMethod);
        }
    }

    void CelestarBallista::onStartShooting(PlayerId player) {
        auto nearestUnit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (nearestUnit) {
            double rangeTo = distanceTo(nearestUnit);
            if (rangeTo < (double) m_stormboltsRapid.range()) {
                PLOG_INFO << name() << " selecting rapid fire.";
                m_stormboltsRapid.activate(true);
                m_stormboltsSingle.activate(false);
            } else {
                PLOG_INFO << name() << " selecting single-shot.";
                m_stormboltsRapid.activate(false);
                m_stormboltsSingle.activate(true);
            }
        }
    }

    int CelestarBallista::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int CelestarBallista::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Burst of Celestial Energy
        if (weapon->isMissile()) {
            return Dice::RollD6();
        }
        return StormcastEternal::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    void CelestarBallista::onRestore() {
        StormcastEternal::onRestore();
        m_stormboltsRapid.activate(false);
        m_stormboltsSingle.activate(true);
    }

} // namespace StormcastEternals
