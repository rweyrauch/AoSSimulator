/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/DragonOgors.h>
#include <UnitFactory.h>
#include <Board.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool DragonOgors::s_registered = false;

    DragonOgors::DragonOgors(Greatfray fray, int numModels, int numPairedWeapons, int numGlaives, int numCrushers, int points) :
            BeastsOfChaosBase("Dragon Ogors", 8, g_wounds, 6, 4, false, points),
            m_pairedAncientWeapons(Weapon::Type::Melee, "Paired Ancient Weapons", 1, 6, 3, 3, 0, 1),
            m_draconicWarglaive(Weapon::Type::Melee, "Draconic War-glaive", 2, 4, 3, 3, -1, 1),
            m_draconicCrusher(Weapon::Type::Melee, "Draconic Crusher", 1, 3, 3, 3, 0, 2),
            m_rakingForeclaws(Weapon::Type::Melee, "Raking Foreclaws", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, THUNDERSCORN, DRAGON_OGORS};
        m_weapons = {&m_pairedAncientWeapons, &m_draconicWarglaive, &m_draconicCrusher, &m_rakingForeclaws};

        for (auto i = 0; i < numPairedWeapons; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_pairedAncientWeapons);
            addModel(model);
        }
        for (auto i = 0; i < numGlaives; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_draconicWarglaive);
            addModel(model);
        }
        for (auto i = 0; i < numCrushers; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_draconicCrusher);
            addModel(model);
        }
    }

    Unit *DragonOgors::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numPairedWeapons = GetIntParam("Paired Ancient Weapons", parameters, numModels);
        int numGlaives = GetIntParam("Draconic War-glaive", parameters, 0);
        int numCrushers = GetIntParam("Draconic Crusher", parameters, 0);
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);

        return new DragonOgors(fray, numModels, numPairedWeapons, numGlaives, numCrushers, ComputePoints(parameters));
    }

    void DragonOgors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Paired Ancient Weapons", g_minUnitSize, g_minUnitSize, g_maxUnitSize,
                                             g_minUnitSize),
                            IntegerParameter("Draconic War-glaive", g_minUnitSize, g_minUnitSize, g_maxUnitSize,
                                             g_minUnitSize),
                            IntegerParameter("Draconic Crusher", g_minUnitSize, g_minUnitSize, g_maxUnitSize,
                                             g_minUnitSize),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Dragon Ogors", factoryMethod);
        }
    }

    Rerolls DragonOgors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Storm Rage
        auto shaggoth = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), DRAGON_OGOR, 12.0);
        if (shaggoth) {
            return Rerolls::Ones;
        }
        return BeastsOfChaosBase::toHitRerolls(weapon, target);
    }

    int DragonOgors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace BeastsOfChaos