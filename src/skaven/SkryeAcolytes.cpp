/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/SkryeAcolytes.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 200;

    bool SkryeAcolytes::s_registered = false;

    Unit *SkryeAcolytes::Create(const ParameterList &parameters) {
        auto unit = new SkryeAcolytes(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int SkryeAcolytes::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void SkryeAcolytes::Init() {
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

            s_registered = UnitFactory::Register("Skrye Acolytes", factoryMethod);
        }
    }

    SkryeAcolytes::SkryeAcolytes(int points) :
            Skaventide("Skrye Acolytes", 6, g_wounds, 4, 6, false, points),
            m_globe(Weapon::Type::Missile, "Poisoned Wind Globe", 8, 1, 4, 4, -2, RAND_D3),
            m_knife(Weapon::Type::Melee, "Rusty Knife", 1, 1, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, SKRYRE_ACOLYTES};
        m_weapons = {&m_globe, &m_knife};

        // Quick-quick Volley!
        m_runAndShoot = true;
    }

    bool SkryeAcolytes::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_globe);
            model->addMeleeWeapon(&m_knife);
            addModel(model);
        }

        return true;
    }

    int SkryeAcolytes::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Skaventide::toHitModifier(weapon, target);

        // Gas Clouds
        if ((weapon->name() == m_globe.name()) && (target->remainingModels() >= 10)) {
            mod++;
        }
        return mod;
    }

} //namespace Skaven
