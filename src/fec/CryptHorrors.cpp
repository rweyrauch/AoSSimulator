/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptHorrors.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = 130 * 4;

    bool CryptHorrors::s_registered = false;

    CryptHorrors::CryptHorrors() :
            FleshEaterCourts("Crypt Horrors", 7, g_wounds, 10, 5, false),
            m_clubsAndTalons(Weapon::Type::Melee, "Clubs and Septic Talons", 1, 3, 4, 3, 0, 2),
            m_clubsAndTalonsHaunter(Weapon::Type::Melee, "Clubs and Septic Talons", 1, 4, 4, 3, 0, 2) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, KNIGHTS, CRYPT_HORRORS};
        m_weapons = {&m_clubsAndTalons, &m_clubsAndTalonsHaunter};
    }

    bool CryptHorrors::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto haunter = new Model(g_basesize, wounds());
        haunter->addMeleeWeapon(&m_clubsAndTalonsHaunter);
        addModel(haunter);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_clubsAndTalons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *CryptHorrors::Create(const ParameterList &parameters) {
        auto unit = new CryptHorrors();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);

        // Can only select delusion if GrandCourt is NoCourt.
        unit->setGrandCourt(court);
        if (court == GrandCourt::None)
            unit->setCourtsOfDelusion(delusion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void CryptHorrors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CryptHorrors::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    CryptHorrors::ComputePoints,
                    {
                            IntegerParameter( "Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Crypt Horrors", factoryMethod);
        }
    }

    Rerolls CryptHorrors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Chosen of the King
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), ABHORRANT, 18.0);
        if (unit != nullptr) {
            return Reroll_Failed;
        }
        return FleshEaterCourts::toHitRerolls(weapon, target);
    }

    void CryptHorrors::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            // Noble Blood
            int woundsHealed = heal(remainingModels());
            if (woundsHealed) {
                //std::cout << "Healed " << woundsHealed << " from Noble Blood." << std::endl;
            }
        }
    }

    Wounds CryptHorrors::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Warrior Elite - one additional damage for each wound roll of 6.
        if (woundRoll == 6) {
            return {weapon->damage() + 1, 0};
        }
        return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int CryptHorrors::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace FleshEaterCourt
