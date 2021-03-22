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
        auto unit = new FomoroidCrusher();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    FomoroidCrusher::FomoroidCrusher() :
            SlavesToDarknessBase("Fomoroid Crusher", 6, g_wounds, 10, 5, false),
            m_hurledTerrain(Weapon::Type::Missile, "Hurled Terrain", 12, 2, 3, 3, -1, 2),
            m_fists(Weapon::Type::Melee, "Crushing Fists", 1, 4, 3, 3, 0, 2) {
        m_keywords = {CHAOS, MORTAL, MONSTER, SLAVES_TO_DARKNESS, FOMOROID_CRUSHER};
        m_weapons = {&m_hurledTerrain, &m_fists};
    }

    bool FomoroidCrusher::configure() {
        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_hurledTerrain);
        model->addMeleeWeapon(&m_fists);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
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

    int FomoroidCrusher::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

}//namespace SlavesToDarkness

