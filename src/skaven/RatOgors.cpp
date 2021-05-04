/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/RatOgors.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 8;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 360;

    bool RatOgors::s_registered = false;

    Unit *RatOgors::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numGuns = GetIntParam("Warpfire Guns", parameters, g_minUnitSize / 2);
        return new RatOgors(numModels, numGuns, ComputePoints(parameters));
    }

    int RatOgors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void RatOgors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Warpfire Guns", g_minUnitSize / 2, g_minUnitSize / 2,
                                             g_maxUnitSize / 2, g_minUnitSize / 2),
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Rat Ogors", factoryMethod);
        }
    }

    RatOgors::RatOgors(int numModels, int numGuns, int points) :
            Skaventide("Rat Ogors", 6, g_wounds, 5, 5, false, points),
            m_gun(Weapon::Type::Missile, "Warpfire Gun", 16, 1, 5, 3, -1, RAND_D3),
            m_clawsBladesAndFangs(Weapon::Type::Melee, "Tearing Claws, Blades and Fangs", 1, 4, 4, 3, -1, 2) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_MOULDER, FIGHTING_BEAST, PACK, RAT_OGORS};
        m_weapons = {&m_gun, &m_clawsBladesAndFangs};

        for (auto i = 0; i < numGuns; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_gun);
            model->addMeleeWeapon(&m_clawsBladesAndFangs);
            addModel(model);
        }
        for (auto i = numGuns; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_clawsBladesAndFangs);
            addModel(model);
        }
    }

    int RatOgors::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Rabid Fury
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_clawsBladesAndFangs.name())) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

} //namespace Skaven
