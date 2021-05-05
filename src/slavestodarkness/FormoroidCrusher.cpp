/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "slavestodarkness/FomoroidCrusher.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 60;
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 100;

    bool FomoroidCrusher::s_registered = false;

    Unit *FomoroidCrusher::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new FomoroidCrusher(legion);
    }

    void FomoroidCrusher::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FomoroidCrusher::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    FomoroidCrusher::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Fomoroid Crusher", factoryMethod);
        }
    }

    FomoroidCrusher::FomoroidCrusher(DamnedLegion legion) :
            SlavesToDarknessBase(legion, "Fomoroid Crusher", 6, g_wounds, 10, 5, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, MONSTER, SLAVES_TO_DARKNESS, FOMOROID_CRUSHER};
        m_weapons = {&m_hurledTerrain, &m_fists};

        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_hurledTerrain);
        model->addMeleeWeapon(&m_fists);
        addModel(model);
    }

    void FomoroidCrusher::onCharged() {
        Unit::onCharged();

        // Rampage
        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            Dice::RollResult rolls;
            Dice::RollD6(m_lastChargeDistance, rolls);

            Wounds wounds = {0, rolls.rollsGE(6)};
            m_meleeTarget->applyDamage(wounds, this);
        }
    }

    void FomoroidCrusher::onStartHero(PlayerId player) {
        Unit::onStartHero(player);
    }

    int FomoroidCrusher::rollChargeDistance() {
        m_lastChargeDistance = SlavesToDarknessBase::rollChargeDistance();
        return m_lastChargeDistance;
    }

    int FomoroidCrusher::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

}//namespace SlavesToDarkness

