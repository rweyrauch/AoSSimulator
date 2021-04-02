/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/GryphHounds.h>
#include <UnitFactory.h>
#include <Board.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 6;
    static const int g_maxUnitSize = 18;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 360;

    bool GryphHounds::s_registered = false;

    GryphHounds::GryphHounds(Stormhost stormhost, int numModels) :
            StormcastEternal(stormhost, "Gryph-hounds", 9, g_wounds, 6, NoSave, false),
            m_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 3, 4, 0, 1),
            m_beakAndClawsAlpha(Weapon::Type::Melee, "Beak and Claws", 1, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, STORMCAST_ETERNAL, GRYPH_HOUNDS};
        m_weapons = {&m_beakAndClaws, &m_beakAndClawsAlpha};

        auto alpha = new Model(g_basesize, wounds());
        alpha->addMeleeWeapon(&m_beakAndClawsAlpha);
        addModel(alpha);

        for (int i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_beakAndClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Unit *GryphHounds::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        return new GryphHounds(stormhost, numModels);
    }

    void GryphHounds::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize,
                                             g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Gryph-hounds", factoryMethod);
        }
    }

    int GryphHounds::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = StormcastEternal::extraAttacks(attackingModel, weapon, target);

        // Loyal Companion
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 6);
        for (auto ip : units) {
            if (ip->hasKeyword(LORD_CASTELLANT) || (ip->hasKeyword(LORD_VERITANT))) {
                attacks += 2;
            }
        }
        return attacks;
    }

    int GryphHounds::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace StormcastEternals