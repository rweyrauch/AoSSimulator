/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Stormvermin.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool Stormvermin::s_registered = false;

    Stormvermin::Stormvermin(int numModels, bool clanshields, int standardBearers, int drummers, int points) :
            Skaventide("Stormvermin", 6, g_wounds, 5, 5, false, points),
            m_rustyHalberd(Weapon::Type::Melee, "Rusty Halberd", 2, 2, 4, 3, -1, 1),
            m_rustyHalberdLeader(Weapon::Type::Melee, "Rusty Halberd", 2, 3, 4, 3, -1, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_VERMINUS, STORMVERMIN};
        m_weapons = {&m_rustyHalberd, &m_rustyHalberdLeader};

        m_clanshields = clanshields;
        m_numStandardBearers = standardBearers;
        m_numDrummers = drummers;

        // Standard Bearers
        m_retreatAndCharge = (standardBearers > 0);

        auto leader = new Model(g_basesize, wounds());
        leader->addMeleeWeapon(&m_rustyHalberdLeader);
        addModel(leader);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_rustyHalberd);
            addModel(model);
        }
    }

    Unit *Stormvermin::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool clanshields = GetBoolParam("Clanshields", parameters, false);
        int standardBearers = GetIntParam("Standard Bearers", parameters, 0);
        int drummers = GetIntParam("Drummers", parameters, 0);
        return new Stormvermin(numModels, clanshields, standardBearers, drummers, ComputePoints(parameters));
    }

    void Stormvermin::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Clanshields"),
                            IntegerParameter("Standard Bearers", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Drummers", 0, 0, g_maxUnitSize / g_minUnitSize, 1)
                    },
                    CHAOS,
                    {SKAVEN}
            };
            s_registered = UnitFactory::Register("Stormvermin", factoryMethod);
        }
    }

    int Stormvermin::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Skaventide::toSaveModifier(weapon, attacker);
        // Clanshields
        if (m_clanshields && remainingModels() >= 10)
            modifier += 1;
        return modifier;
    }

    int Stormvermin::runModifier() const {
        int modifier = Skaventide::runModifier();
        // Drummer
        if (m_numDrummers > 0)
            modifier += 2;
        return modifier;
    }

    int Stormvermin::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Skaven
