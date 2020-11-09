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
    static const int g_basesize = 50;
    static const int g_wounds = 3;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 320;

    bool RipperdactylRiders::s_registered = false;

    RipperdactylRiders::RipperdactylRiders() :
            SeraphonBase("Ripperdactyl Riders", 12, g_wounds, 5, 5, true),
            m_spear(Weapon::Type::Melee, "Moonstone Warspear", 1, 1, 4, 4, 0, 1),
            m_spearAlpha(Weapon::Type::Melee, "Moonstone Warspear", 1, 2, 4, 4, 0, 1),
            m_jaws(Weapon::Type::Melee, "Tearing Jaws", 1, 3, 4, 3, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, RIPPERDACTYL, RIPPERDACTYL_RIDERS};
        m_weapons = {&m_spear, &m_spearAlpha, &m_jaws};
        m_hasMount = true;
    }

    bool RipperdactylRiders::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        // Add the Alpha
        auto alpha = new Model(g_basesize, wounds());
        alpha->addMeleeWeapon(&m_spearAlpha);
        alpha->addMeleeWeapon(&m_jaws);
        addModel(alpha);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_spear);
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *RipperdactylRiders::Create(const ParameterList &parameters) {
        auto unit = new RipperdactylRiders();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
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
            return Reroll_Failed;
        }

        return Unit::toHitRerolls(weapon, target);
    }

} //namespace Seraphon
