/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/IcefallYhetees.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 440;

    bool IcefallYhetees::s_registered = false;

    bool IcefallYhetees::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *IcefallYhetees::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            int numModels = GetIntParam("Models", parameters, g_minUnitSize);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            return new IcefallYhetees(tribe, numModels);
        }
        return nullptr;
    }

    void IcefallYhetees::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    IcefallYhetees::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    IcefallYhetees::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Icefall Yhetees", factoryMethod);
        }
    }

    IcefallYhetees::IcefallYhetees(Mawtribe tribe, int numModels) :
            MawtribesBase(tribe, "Icefall Yhetees", 9, g_wounds, 6, 6, false) {
        m_keywords = {DESTRUCTION, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, ICEFALL_YHETESS};
        m_weapons = {&m_clawsAndClubs};

        // Bounding Leaps
        m_pileInMove = 6;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_clawsAndClubs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    int IcefallYhetees::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int IcefallYhetees::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);

        // Aura of Frost
        if (!weapon->isMissile()) mod--;

        return mod;
    }

} // namespace OgorMawtribes
