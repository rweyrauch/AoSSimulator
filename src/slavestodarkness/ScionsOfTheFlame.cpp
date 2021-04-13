/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ScionsOfTheFlame.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 8;
    static const int g_maxUnitSize = 32;
    static const int g_pointsPerBlock = 0;
    static const int g_pointsMaxUnitSize = 0;

    bool ScionsOfTheFlame::s_registered = false;

    Unit *ScionsOfTheFlame::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new ScionsOfTheFlame(legion, numModels);
    }

    void ScionsOfTheFlame::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };

            s_registered = UnitFactory::Register("Scions of the Flame", factoryMethod);
        }
    }

    ScionsOfTheFlame::ScionsOfTheFlame(DamnedLegion legion, int numModels) :
            SlavesToDarknessBase("Scions of the Flame", 6, g_wounds, 5, 6, false) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, SCIONS_OF_THE_FLAME};
        m_weapons = {&m_pots, &m_scionWeapons, &m_scionWeaponsLeaders};

        setDamnedLegion(legion);

        auto lord = new Model(g_basesize, wounds());
        lord->addMissileWeapon(&m_pots);
        lord->addMeleeWeapon(&m_scionWeaponsLeaders);
        lord->setName("Blazing Lord");
        addModel(lord);

        auto immolator = new Model(g_basesize, wounds());
        immolator->addMissileWeapon(&m_pots);
        immolator->addMeleeWeapon(&m_scionWeaponsLeaders);
        immolator->setName("Immolator");
        addModel(immolator);

        auto champion = new Model(g_basesize, wounds() + 1);
        champion->addMissileWeapon(&m_pots);
        champion->addMeleeWeapon(&m_scionWeapons);
        champion->setName("Brazen Champion");
        addModel(champion);

        auto priest = new Model(g_basesize, wounds());
        priest->addMissileWeapon(&m_pots);
        priest->addMeleeWeapon(&m_scionWeapons);
        priest->setName("Inferno Priest");
        addModel(priest);

        for (auto i = (int) m_models.size(); i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_pots);
            model->addMeleeWeapon(&m_scionWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    int ScionsOfTheFlame::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls ScionsOfTheFlame::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (weapon->name() == m_pots.name()) {
            if (isNamedModelAlive("Inferno Priest"))
                return Rerolls::Ones;
        }
        return SlavesToDarknessBase::toHitRerolls(weapon, target);
    }

    int ScionsOfTheFlame::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        auto extraHits = SlavesToDarknessBase::generateHits(unmodifiedHitRoll, weapon, unit);

        // All Shall Burn
        if ((unmodifiedHitRoll == 6) && weapon->isMissile()) {
            extraHits++;
        }
        return extraHits;
    }

} //SlavesToDarkness