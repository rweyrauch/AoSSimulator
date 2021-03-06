/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/ScylaAnfingrimm.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 60;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 100;

    bool ScylaAnfingrimm::s_registered = false;

    ScylaAnfingrimm::ScylaAnfingrimm(SlaughterHost host) :
            KhorneBase("Scyla Anfingrimm", 8, g_wounds, 9, 5, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, SCYLA_ANFINGRIMM};
        m_weapons = {&m_brutalFists, &m_serpentineTail};
        m_battleFieldRole = Role::Leader;

        // Brass Collar of Khorne
        m_totalUnbinds = 1;

        // Beastial Leap
        m_pileInMove = 8;

        setSlaughterHost(host);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_brutalFists);
        model->addMeleeWeapon(&m_serpentineTail);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *ScylaAnfingrimm::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        return new ScylaAnfingrimm(host);
    }

    void ScylaAnfingrimm::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ScylaAnfingrimm::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    ScylaAnfingrimm::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Scyla Anfingrimm", factoryMethod);
        }
    }

    int ScylaAnfingrimm::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = KhorneBase::extraAttacks(attackingModel, weapon, target);
        // Raging Fury
        attacks += (g_wounds - remainingWounds());

        return attacks;
    }

    int ScylaAnfingrimm::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne