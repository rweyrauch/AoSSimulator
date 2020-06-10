/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/CypherLords.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 8;
    static const int MAX_UNIT_SIZE = 32;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool CypherLords::s_registered = false;

    Unit *CypherLords::Create(const ParameterList &parameters) {
        auto unit = new CypherLords();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, Ravagers);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void CypherLords::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CypherLords::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    CypherLords::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Cypher Lords", factoryMethod);
        }
    }

    CypherLords::CypherLords() :
            SlavesToDarknessBase("Cypher Lords", 6, WOUNDS, 5, 6, false),
            m_throwingStars(Weapon::Type::Missile, "Throwing Stars and Chakrams", 8, 1, 4, 5, 0, 1),
            m_exoticBlades(Weapon::Type::Melee, "Exotic Blades", 1, 1, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, CYPHER_LORDS};
        m_weapons = {&m_throwingStars, &m_exoticBlades};
    }

    bool CypherLords::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto thrallmaster = new Model(BASESIZE, wounds());
        thrallmaster->addMissileWeapon(&m_throwingStars);
        thrallmaster->addMeleeWeapon(&m_exoticBlades);
        thrallmaster->setName("Thrallmaster");
        addModel(thrallmaster);

        auto luminate = new Model(BASESIZE, wounds());
        luminate->addMissileWeapon(&m_throwingStars);
        luminate->addMeleeWeapon(&m_exoticBlades);
        luminate->setName("Luminate");
        addModel(luminate);

        for (auto i = 2; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_throwingStars);
            model->addMeleeWeapon(&m_exoticBlades);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int CypherLords::chargeModifier() const {
        int modifier = Unit::chargeModifier();
        if (m_hasLuminate) {
            modifier += 1;
        }
        return modifier;
    }

    void CypherLords::onWounded() {
        Unit::onWounded();

        // Check for Thrallmaster and Luminate
        for (const auto &ip : m_models) {
            if (ip->slain() && (ip->getName() == "Thrallmaster")) {
                m_hasThrallmaster = false;
            }
            if (ip->slain() && (ip->getName() == "Luminate")) {
                m_hasLuminate = false;
            }
        }
    }

    int CypherLords::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //SlavesToDarkness