/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Gnoblars.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 20;
    static const int g_maxUnitSize = 60;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 270;

    bool Gnoblars::s_registered = false;

    bool Gnoblars::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *Gnoblars::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto numModels = GetIntParam("Models", parameters, g_minUnitSize);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            return new Gnoblars(tribe, numModels);
        }
        return nullptr;
    }

    void Gnoblars::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Gnoblars::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    Gnoblars::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Gnoblars", factoryMethod);
        }
    }

    Gnoblars::Gnoblars(Mawtribe tribe, int numModels) :
            MawtribesBase(tribe, "Gnoblars", 5, g_wounds, 4, 6, false) {
        m_keywords = {DESTRUCTION, GROT, OGOR_MAWTRIBES, GUTBUSTERS, GNOBLARS};
        m_weapons = {&m_sharpStuff, &m_motleyWeapons, &m_motleyWeaponsBiter};

        auto biter = new Model(g_basesize, wounds());
        biter->addMissileWeapon(&m_sharpStuff);
        biter->addMeleeWeapon(&m_motleyWeaponsBiter);
        addModel(biter);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_sharpStuff);
            model->addMeleeWeapon(&m_motleyWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    int Gnoblars::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Gnoblars::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // Screeching Horde
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        if (remainingModels() >= 10) attacks++;
        return attacks;
    }

} // namespace OgorMawtribes
