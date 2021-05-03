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
    static const int g_pointsPerBlock = 150;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool MyrmideshPainbringers::s_registered = false;

    MyrmideshPainbringers::MyrmideshPainbringers(Host host, int numModels, int points) :
            SlaaneshBase(host, "Myrmidesh Painbringers", 6, g_wounds, 7, 4, false, points),
            m_scimitar(Weapon::Type::Missile, "Scimitar", 1, 2, 3, 3, -1, 1),
            m_scimitarMaster(Weapon::Type::Melee, "Scimitar", 1, 3, 3, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, SEEKERS, MYRMIDESH_PAINBRINGERS};
        m_weapons = {&m_scimitar, &m_scimitarMaster};

        // Flawless Accuracy
        m_runAndCharge = true;

        auto tempter = new Model(g_basesize, wounds());
        tempter->addMeleeWeapon(&m_scimitarMaster);
        tempter->setName("Painmaster");
        addModel(tempter);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_scimitar);
            addModel(model);
        }
    }

    Unit *MyrmideshPainbringers::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new MyrmideshPainbringers(host, numModels, ComputePoints(parameters));
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

    int MyrmideshPainbringers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Wounds
    MyrmideshPainbringers::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Dance of the Wailing Blade
        if ((woundRoll == 6) && !weapon->isMissile()) {
            return {0, 1};
        }
        return SlaaneshBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Rerolls MyrmideshPainbringers::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Painbringer Shields
        if (!weapon->isMissile()) {
            return Rerolls::Failed;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

} // namespace Slaanesh