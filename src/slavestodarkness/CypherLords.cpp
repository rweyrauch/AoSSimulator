/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "slavestodarkness/CypherLords.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 8;
    static const int g_maxUnitSize = 32;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 280;

    bool CypherLords::s_registered = false;

    Unit *CypherLords::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new CypherLords(legion, numModels, ComputePoints(parameters));
    }

    void CypherLords::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CypherLords::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    CypherLords::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Cypher Lords", factoryMethod);
        }
    }

    CypherLords::CypherLords(DamnedLegion legion, int numModels, int points) :
            SlavesToDarknessBase(legion, "Cypher Lords", 6, g_wounds, 5, 6, false, points),
            m_throwingStars(Weapon::Type::Missile, "Throwing Stars and Chakrams", 8, 1, 4, 5, 0, 1),
            m_exoticBlades(Weapon::Type::Melee, "Exotic Blades", 1, 1, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, CYPHER_LORDS};
        m_weapons = {&m_throwingStars, &m_exoticBlades};

        auto thrallmaster = new Model(g_basesize, wounds());
        thrallmaster->addMissileWeapon(&m_throwingStars);
        thrallmaster->addMeleeWeapon(&m_exoticBlades);
        thrallmaster->setName("Thrallmaster");
        addModel(thrallmaster);

        auto luminate = new Model(g_basesize, wounds());
        luminate->addMissileWeapon(&m_throwingStars);
        luminate->addMeleeWeapon(&m_exoticBlades);
        luminate->setName("Luminate");
        addModel(luminate);

        for (auto i = 2; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_throwingStars);
            model->addMeleeWeapon(&m_exoticBlades);
            addModel(model);
        }
    }

    int CypherLords::chargeModifier() const {
        int modifier = SlavesToDarknessBase::chargeModifier();
        if (isNamedModelAlive("Luminate")) {
            modifier += 1;
        }
        return modifier;
    }

    int CypherLords::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void CypherLords::onStartCombat(PlayerId player) {
        SlavesToDarknessBase::onStartCombat(player);

        // Shattered Gloom Globe
        if (owningPlayer() == player) {
            if (isNamedModelAlive("Thrallmaster")) {
                auto enemy = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
                if (enemy && (distanceTo(enemy) < 3.0)) {
                    if (Dice::RollD6() >= 4) {
                        enemy->buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                        enemy->buffModifier(Attribute::To_Hit_Missile, -1, {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                    }
                }
            }
        }
    }

} //SlavesToDarkness