/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Dawnriders.h>
#include <UnitFactory.h>
#include <iostream>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int BASESIZE = 65; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 0;
    static const int POINTS_MAX_UNIT_SIZE = 0;

    bool Dawnriders::s_registered = false;

    Dawnriders::Dawnriders() :
            LuminethBase("Vanari Dawnriders", 14, WOUNDS, 7, 4, false),
            m_guardiansSword(Weapon::Type::Melee, "Guardian's Sword", 1, 2, 3, 4, -1, 1),
            m_lance(Weapon::Type::Melee, "Sunmetal Lance", 2, 1, 3, 4, 0, 1),
            m_hooves(Weapon::Type::Melee, "Dashing Hooves", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, DAWNRIDERS};
        m_weapons = {&m_guardiansSword, &m_lance, &m_hooves};
        m_battleFieldRole = Role::Battleline;
    }

    bool Dawnriders::configure(int numModels, bool standardBearer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto master = new Model(BASESIZE, wounds());
        master->addMeleeWeapon(&m_guardiansSword);
        master->addMeleeWeapon(&m_hooves);
        master->setName("Steedmaster");
        addModel(master);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_lance);
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        m_standardBearer = standardBearer;
        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Dawnriders::Create(const ParameterList &parameters) {
        auto unit = new Dawnriders();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto standard = GetBoolParam("Standard Bearer", parameters, true);

        bool ok = unit->configure(numModels, standard);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Dawnriders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Standard Bearer")
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Vanari Dawnriders", factoryMethod);
        }
    }

    int Dawnriders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace LuminethRealmLords