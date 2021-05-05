/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/UntamedBeasts.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 9;
    static const int g_maxUnitSize = 36;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 280;

    bool UntamedBeasts::s_registered = false;

    Unit *UntamedBeasts::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new UntamedBeasts(legion, numModels, ComputePoints(parameters));
    }

    void UntamedBeasts::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    UntamedBeasts::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    UntamedBeasts::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };

            s_registered = UnitFactory::Register("Untamed Beasts", factoryMethod);
        }
    }

    UntamedBeasts::UntamedBeasts(DamnedLegion legion, int numModels, int points) :
            SlavesToDarknessBase(legion, "Untamed Beasts", 6, g_wounds, 5, 6, false, points) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, UNTAMED_BEASTS};
        m_weapons = {&m_harpoonFirstFang, &m_huntingWeapons, &m_huntingWeaponsHeartEater};

        // Unleash the Beast
        m_runAndCharge = true;

        auto hearteater = new Model(g_basesize, wounds());
        hearteater->addMeleeWeapon(&m_huntingWeaponsHeartEater);
        hearteater->setName("Heart-eater");
        addModel(hearteater);

        auto firstfang = new Model(g_basesize, wounds());
        firstfang->addMissileWeapon(&m_harpoonFirstFang);
        firstfang->addMeleeWeapon(&m_huntingWeapons);
        firstfang->setName("First Fang");
        addModel(firstfang);

        auto prowler = new Model(g_basesize, 2);
        prowler->addMeleeWeapon(&m_huntingWeapons);
        prowler->setName("Rocktusk Prowler");
        addModel(prowler);

        for (auto i = 3; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_huntingWeapons);
            addModel(model);
        }
    }

    int UntamedBeasts::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //SlavesToDarkness