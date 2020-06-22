/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/RipperdactylRiders.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int BASESIZE = 50;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    bool RipperdactylRiders::s_registered = false;

    RipperdactylRiders::RipperdactylRiders() :
            SeraphonBase("Ripperdactyl Riders", 12, WOUNDS, 5, 5, true),
            m_spear(Weapon::Type::Melee, "Moonstone Warspear", 1, 1, 4, 4, 0, 1),
            m_spearAlpha(Weapon::Type::Melee, "Moonstone Warspear", 1, 2, 4, 4, 0, 1),
            m_jaws(Weapon::Type::Melee, "Tearing Jaws", 1, 3, 4, 3, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, RIPPERDACTYL, RIPPERDACTYL_RIDERS};
        m_weapons = {&m_spear, &m_spearAlpha, &m_jaws};
        m_hasMount = true;
    }

    bool RipperdactylRiders::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        // Add the Alpha
        auto alpha = new Model(BASESIZE, wounds());
        alpha->addMeleeWeapon(&m_spearAlpha);
        alpha->addMeleeWeapon(&m_jaws);
        addModel(alpha);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_spear);
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *RipperdactylRiders::Create(const ParameterList &parameters) {
        auto unit = new RipperdactylRiders();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void RipperdactylRiders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Ripperdactyl Riders", factoryMethod);
        }
    }

    int RipperdactylRiders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int RipperdactylRiders::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Voracious Appetite
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_jaws.name())) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    Rerolls RipperdactylRiders::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (!m_usedToad && (weapon->name() == m_jaws.name())) {
            m_usedToad = true;
            return RerollFailed;
        }

        return Unit::toHitRerolls(weapon, target);
    }

} //namespace Seraphon
