/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "slavestodarkness/SpireTyrants.h"
#include "SlavesToDarknessPrivate.h"


namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 9;
    static const int g_maxUnitSize = 36;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 280;

    bool SpireTyrants::s_registered = false;

    Unit *SpireTyrants::Create(const ParameterList &parameters) {
        auto unit = new SpireTyrants();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int SpireTyrants::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void SpireTyrants::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpireTyrants::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    SpireTyrants::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Spire Tyrants", factoryMethod);
        }
    }

    SpireTyrants::SpireTyrants() :
            SlavesToDarknessBase("Spire Tyrants", 6, g_wounds, 5, 5, false),
            m_gladiatorialWeapons(Weapon::Type::Melee, "Gladiatorial Weapons", 1, 1, 4, 4, 0, 1),
            m_gladiatorialWeaponsChampion(Weapon::Type::Melee, "Gladiatorial Weapons", 1, 3, 4, 4, 0, 1),
            m_gladiatorialWeaponsHeadclaimer(Weapon::Type::Melee, "Gladiatorial Weapons", 1, 1, 4, 4, 0, 2),
            m_gladiatorialWeaponsDestroyer(Weapon::Type::Melee, "Gladiatorial Weapons", 1, 3, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, SPIRE_TYRANTS};
    }

    bool SpireTyrants::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto champion = new Model(g_basesize, wounds());
        champion->addMeleeWeapon(&m_gladiatorialWeaponsChampion);
        champion->setName("Pit Champion");
        addModel(champion);

        auto headclaimer = new Model(g_basesize, wounds());
        headclaimer->addMeleeWeapon(&m_gladiatorialWeaponsHeadclaimer);
        headclaimer->setName("Headclaimer");
        addModel(headclaimer);

        auto destroyer = new Model(g_basesize, wounds());
        destroyer->addMeleeWeapon(&m_gladiatorialWeaponsDestroyer);
        destroyer->setName("Bestigor Destroyer");
        addModel(destroyer);

        for (auto i = 3; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_gladiatorialWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int SpireTyrants::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);
        // Pit Fighters
        if (m_charged) mod++;

        return mod;
    }

}//namespace SlavesToDarkness
