/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptGhouls.h>
#include <UnitFactory.h>
#include <Board.h>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 360;

    bool CryptGhouls::s_registered = false;

    CryptGhouls::CryptGhouls(int points) :
            FleshEaterCourts("Crypt Ghouls", 6, g_wounds, 10, 6, false, points),
            m_teethAndClaws(Weapon::Type::Melee, "Sharpened Teeth and Filthy Claws", 1, 2, 4, 4, 0, 1),
            m_teethAndClawsGhast(Weapon::Type::Melee, "Sharpened Teeth and Filthy Claws", 1, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, SERFS, CRYPT_GHOULS};
        m_weapons = {&m_teethAndClaws, &m_teethAndClawsGhast};
        m_battleFieldRole = Role::Battleline;
    }

    bool CryptGhouls::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto ghast = new Model(g_basesize, wounds());
        ghast->addMeleeWeapon(&m_teethAndClawsGhast);
        addModel(ghast);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_teethAndClaws);
            addModel(model);
        }

        return true;
    }

    Unit *CryptGhouls::Create(const ParameterList &parameters) {
        auto unit = new CryptGhouls(ComputePoints(parameters));
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

    void CryptGhouls::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CryptGhouls::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    CryptGhouls::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Crypt Ghouls", factoryMethod);
        }
    }

    int CryptGhouls::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = FleshEaterCourts::extraAttacks(attackingModel, weapon, target);

        // Boundless Ferocity
        if (remainingModels() >= 20) {
            attacks += 1;
        }

        return attacks;
    }

    Rerolls CryptGhouls::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Royal Approval
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), ABHORRANT, 18.0);
        if (unit != nullptr) {
            return Rerolls::Ones;
        }
        return FleshEaterCourts::toHitRerolls(weapon, target);
    }

    int CryptGhouls::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace FleshEaterCourt
