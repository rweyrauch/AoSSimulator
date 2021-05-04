/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/NightRunners.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool NightRunners::s_registered = false;

    NightRunners::NightRunners(int numModels, int points) :
            Skaventide("Night Runners", 7, g_wounds, 4, 6, false, points),
            m_throwingWeapons(Weapon::Type::Missile, "Eshin Throwing Weapons", 12, 1, 4, 5, 0, 1),
            m_stabbingBlade(Weapon::Type::Melee, "Stabbing Blade", 1, 1, 4, 4, 0, 1),
            m_stabbingBladeLeader(Weapon::Type::Melee, "Stabbing Blade", 1, 1, 4, 4, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_ESHIN, NIGHT_RUNNERS};
        m_weapons = {&m_throwingWeapons, &m_stabbingBlade, &m_stabbingBladeLeader};

        // Running Death
        m_runAndShoot = true;

        auto leader = new Model(g_basesize, wounds());
        leader->addMissileWeapon(&m_throwingWeapons);
        leader->addMeleeWeapon(&m_stabbingBladeLeader);
        addModel(leader);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_throwingWeapons);
            model->addMeleeWeapon(&m_stabbingBlade);
            addModel(model);
        }
    }

    Unit *NightRunners::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new NightRunners(numModels, ComputePoints(parameters));
    }

    void NightRunners::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    CHAOS,
                    {SKAVEN}
            };
            s_registered = UnitFactory::Register("Night Runners", factoryMethod);
        }
    }

    int NightRunners::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Throwing Weapons
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_throwingWeapons.name())) {
            return weapon->attacks() + 1;
        }
        return Skaventide::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int NightRunners::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Skaven
