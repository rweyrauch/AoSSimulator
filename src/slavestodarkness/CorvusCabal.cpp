/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/CorvusCabal.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 9;
    static const int MAX_UNIT_SIZE = 36;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool CorvusCabal::s_registered = false;

    Unit *CorvusCabal::Create(const ParameterList &parameters) {
        auto unit = new CorvusCabal();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void CorvusCabal::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CorvusCabal::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    CorvusCabal::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Corvus Cabal", factoryMethod);
        }
    }

    CorvusCabal::CorvusCabal() :
            SlavesToDarknessBase("Corvus Cabal", 8, WOUNDS, 5, 6, true),
            m_ravenDarts(Weapon::Type::Missile, "Raven Darts", 8, 1, 4, 5, 0, 1),
            m_corvusWeapons(Weapon::Type::Melee, "Corvus Weapons", 1, 1, 4, 4, 0, 1),
            m_corvusWeaponsLeader(Weapon::Type::Melee, "Corvus Weapons (Shadow Piercer)", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, CORVUS_CABAL};
        m_weapons = {&m_ravenDarts, &m_corvusWeapons, &m_corvusWeaponsLeader};
    }

    bool CorvusCabal::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto piercer = new Model(BASESIZE, wounds());
        piercer->addMissileWeapon(&m_ravenDarts);
        piercer->addMeleeWeapon(&m_corvusWeaponsLeader);
        piercer->setName("Shadow Piercer");
        addModel(piercer);

        auto talon = new Model(BASESIZE, wounds());
        talon->addMissileWeapon(&m_ravenDarts);
        talon->addMeleeWeapon(&m_corvusWeapons);
        talon->setName("Shrike Talon");
        addModel(talon);

        for (auto i = 2; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_ravenDarts);
            model->addMeleeWeapon(&m_corvusWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Rerolls CorvusCabal::chargeRerolls() const {
        if (m_hasShrikeTalon) {
            return RerollOnes;
        }
        return Unit::chargeRerolls();
    }

    void CorvusCabal::onWounded() {
        Unit::onWounded();

        // Check for Shrike Talon
        for (const auto &ip : m_models) {
            if (ip->slain() && (ip->getName() == "Shrike Talon")) {
                m_hasShrikeTalon = false;
                break;
            }
        }
    }

    int CorvusCabal::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //SlavesToDarkness