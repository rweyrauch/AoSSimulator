/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/MymideshPainbringers.h>
#include <Weapon.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 0;
    static const int g_pointsMaxUnitSize = 0;

    bool MyrmideshPainbringers::s_registered = false;

    MyrmideshPainbringers::MyrmideshPainbringers() :
            SlaaneshBase("Myrmidesh Painbringers", 6, g_wounds, 7, 4, false),
            m_scimitar(Weapon::Type::Missile, "Scimitar", 1, 2, 3, 3, -1, 1),
            m_scimitarMaster(Weapon::Type::Melee, "Scimitar", 1, 3, 3, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, SEEKERS, MYRMIDESH_PAINBRINGERS};
        m_weapons = {&m_scimitar, &m_scimitarMaster};

        // Flawless Accuracy
        m_runAndCharge = true;
    }

    bool MyrmideshPainbringers::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto tempter = new Model(g_basesize, wounds());
        tempter->addMeleeWeapon(&m_scimitarMaster);
        tempter->setName("Painmaster");
        addModel(tempter);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_scimitar);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *MyrmideshPainbringers::Create(const ParameterList &parameters) {
        auto unit = new MyrmideshPainbringers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MyrmideshPainbringers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Myrmidesh Painbringers", factoryMethod);
        }
    }

    int MyrmideshPainbringers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Wounds MyrmideshPainbringers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Dance of the Wailing Blade
        if ((woundRoll == 6) && !weapon->isMissile()) {
            return {0, 1};
        }
        return UnitModifierInterface::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls MyrmideshPainbringers::toSaveRerolls(const Weapon *weapon) const {
        // Painbringer Shields
        if (!weapon->isMissile()) {
            return Reroll_Failed;
        }
        return Unit::toSaveRerolls(weapon);
    }

} // namespace Slaanesh