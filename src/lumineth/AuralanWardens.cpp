/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/AuralanWardens.h>
#include <UnitFactory.h>
#include <iostream>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 0;
    static const int POINTS_MAX_UNIT_SIZE = 0;

    bool AuralanWardens::s_registered = false;

    AuralanWardens::AuralanWardens() :
            LuminethBase("Vanari Auralan Wardens", 6, WOUNDS, 6, 4, false),
            m_championsBlade(Weapon::Type::Melee, "Champion's Blade", 1, 2, 3, 4, -1, 1),
            m_wardensPike(Weapon::Type::Melee, "Warden's Pike", 3, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, AURALAN_WARDENS};
        m_weapons = {&m_championsBlade, &m_wardensPike};
        m_battleFieldRole = Role::Battleline;
    }

    bool AuralanWardens::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto highWarden = new Model(BASESIZE, wounds());
        highWarden->addMeleeWeapon(&m_championsBlade);
        highWarden->setName("High Warden");
        addModel(highWarden);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_wardensPike);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *AuralanWardens::Create(const ParameterList &parameters) {
        auto unit = new AuralanWardens();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuralanWardens::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Vanari Auralan Wardens", factoryMethod);
        }
    }

    int AuralanWardens::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace LuminethRealmLords