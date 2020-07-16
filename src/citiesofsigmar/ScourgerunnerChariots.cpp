/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/ScourgerunnerChariots.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 6;
    static const int MIN_UNIT_SIZE = 1;
    static const int MAX_UNIT_SIZE = 3;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    bool ScourgerunnerChariots::s_registered = false;

    Unit *ScourgerunnerChariots::Create(const ParameterList &parameters) {
        auto unit = new ScourgerunnerChariots();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string ScourgerunnerChariots::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int ScourgerunnerChariots::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void ScourgerunnerChariots::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ScourgerunnerChariots::Create,
                    ScourgerunnerChariots::ValueToString,
                    ScourgerunnerChariots::EnumStringToInt,
                    ScourgerunnerChariots::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Scourgerunner Chariots", factoryMethod);
        }
    }

    ScourgerunnerChariots::ScourgerunnerChariots() :
            CitizenOfSigmar("Scourgerunner Chariots", 12, WOUNDS, 6, 5, false),
            m_harpoon(Weapon::Type::Missile, "Ravager Harpoon", 18, 2, 3, 3, -1, RAND_D3),
            m_crossbow(Weapon::Type::Missile, "Repeater Crossbow", 16, 4, 5, 4, 0, 1),
            m_hookSpear(Weapon::Type::Melee, "Hook-spear", 2, 2, 4, 4, 0, 1),
            m_bite(Weapon::Type::Melee, "Vicious Bit", 1, 4, 4, 5, 0, 1),
            m_harpoonMaster(Weapon::Type::Missile, "Ravager Harpoon", 18, 2, 2, 3, -1, RAND_D3),
            m_crossbowMaster(Weapon::Type::Missile, "Repeater Crossbow", 16, 4, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, SCOURGE_PRIVATEERS, SCOURGERUNNER_CHARIOTS};
        m_weapons = {&m_harpoon, &m_crossbow, &m_hookSpear, &m_bite, &m_harpoonMaster, &m_crossbowMaster};
        m_hasMount = true;
    }

    bool ScourgerunnerChariots::configure(int numModels) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        // Add the Master
        auto bossModel = new Model(BASESIZE, wounds());
        bossModel->addMissileWeapon(&m_crossbowMaster);
        bossModel->addMissileWeapon(&m_harpoonMaster);
        bossModel->addMeleeWeapon(&m_hookSpear);
        bossModel->addMeleeWeapon(&m_bite);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_crossbow);
            model->addMissileWeapon(&m_harpoon);
            model->addMeleeWeapon(&m_hookSpear);
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds
    ScourgerunnerChariots::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Lay the Beast Low
        if ((hitRoll == 6) && (weapon->name() == m_harpoon.name())) {
            return {0, Dice::rollD3()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ScourgerunnerChariots::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar