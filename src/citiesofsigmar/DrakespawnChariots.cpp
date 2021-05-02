/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/DrakespawnChariots.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 6;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 210;

    bool DrakespawnChariots::s_registered = false;

    Unit *DrakespawnChariots::Create(const ParameterList &parameters) {
        auto unit = new DrakespawnChariots(ComputePoints(parameters));

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string DrakespawnChariots::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int DrakespawnChariots::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void DrakespawnChariots::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DrakespawnChariots::Create,
                    DrakespawnChariots::ValueToString,
                    DrakespawnChariots::EnumStringToInt,
                    DrakespawnChariots::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Drakespawn Chariots", factoryMethod);
        }
    }

    DrakespawnChariots::DrakespawnChariots(int points) :
            CitizenOfSigmar("Drakespawn Chariots", 10, g_wounds, 7, 4, false, points),
            m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 4, 5, 4, 0, 1),
            m_spear(Weapon::Type::Melee, "Barbed Spear", 2, 2, 3, 4, -1, 1),
            m_jaws(Weapon::Type::Melee, "Ferocious Jaws", 1, 4, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, ORDER_SERPENTIS, DRAKESPAWN_CHARIOTS};
        m_weapons = {&m_crossbow, &m_spear, &m_jaws};
        m_hasMount = true;
        m_jaws.setMount(true);
    }

    bool DrakespawnChariots::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_crossbow);
            model->addMeleeWeapon(&m_spear);
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }

        return true;
    }

    int DrakespawnChariots::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void DrakespawnChariots::onCharged() {
        Unit::onCharged();

        // Scythed Runners
        for (int i = 0; i < remainingModels(); i++) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && distanceTo(unit) <= 1.0) {
                if (Dice::RollD6() >= 2) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }
    }

} // namespace CitiesOfSigmar