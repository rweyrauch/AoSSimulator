/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GutterRunners.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 200;

    bool GutterRunners::s_registered = false;

    GutterRunners::GutterRunners(int points) :
            Skaventide("Gutter Runners", 7, g_wounds, 5, 5, false, points),
            m_throwingStars(Weapon::Type::Missile, "Eshin Throwing Stars", 12, 2, 4, 5, 0, 1),
            m_punchDaggerAndBlade(Weapon::Type::Melee, "Punch Daggar and Blade", 1, 2, 3, 4, -1, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_ESHIN, GUTTER_RUNNERS};
        m_weapons = {&m_throwingStars, &m_punchDaggerAndBlade};
    }

    bool GutterRunners::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        // Running Death
        m_runAndShoot = true;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_throwingStars);
            model->addMeleeWeapon(&m_punchDaggerAndBlade);
            addModel(model);
        }

        return true;
    }

    Unit *GutterRunners::Create(const ParameterList &parameters) {
        auto unit = new GutterRunners(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GutterRunners::Init() {
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
            s_registered = UnitFactory::Register("Gutter Runners", factoryMethod);
        }
    }

    int GutterRunners::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Throwing Stars
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_throwingStars.name())) {
            return weapon->attacks() + 1;
        }
        return Skaventide::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int GutterRunners::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;

    }

} // namespace Skaven
