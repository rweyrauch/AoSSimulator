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
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 6;
    static const int MAX_UNIT_SIZE = 18;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 420;

    bool GryphHounds::s_registered = false;

    GryphHounds::GryphHounds() :
            StormcastEternal("Gryph-hounds", 9, WOUNDS, 6, NoSave, false),
            m_beakAndClaws(Weapon::Type::Melee, "Beak and Claws", 1, 2, 3, 4, 0, 1),
            m_beakAndClawsAlpha(Weapon::Type::Melee, "Beak and Claws", 1, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, STORMCAST_ETERNAL, GRYPH_HOUNDS};
        m_weapons = {&m_beakAndClaws, &m_beakAndClawsAlpha};
    }

    bool GryphHounds::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto alpha = new Model(BASESIZE, wounds());
        alpha->addMeleeWeapon(&m_beakAndClawsAlpha);
        addModel(alpha);

        for (int i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_beakAndClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *GryphHounds::Create(const ParameterList &parameters) {
        auto unit = new GryphHounds();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GryphHounds::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE,
                                    MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace StormcastEternals