/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Daemonettes.h>
#include <Weapon.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 330;


    bool Daemonettes::s_registered = false;

    Daemonettes::Daemonettes() :
            SlaaneshBase("Daemonettes", 6, g_wounds, 10, 5, false),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 2, 4, 4, -1, 1),
            m_piercingClawsAlluress(Weapon::Type::Melee, "Piercing Claws", 1, 3, 4, 4, -1, 1) {
        m_keywords = {CHAOS, DAEMON, SLAANESH, HEDONITE, DAEMONETTES};
        m_weapons = {&m_piercingClaws, &m_piercingClawsAlluress};
        m_battleFieldRole = Battleline;

        // Lithe and Swift
        m_runAndCharge = true;

        s_globalBattleshockReroll.connect(this, &Daemonettes::hornblowerBattleshockReroll, &m_hornblowerSlot);
    }

    Daemonettes::~Daemonettes() {
        m_hornblowerSlot.disconnect();
    }

    bool Daemonettes::configure(int numModels, bool iconBearer, bool bannerBearer, bool hornblower) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_bannerBearer = bannerBearer;
        m_hornblower = hornblower;

        // Add the Alluress
        auto reaperModel = new Model(g_basesize, wounds());
        reaperModel->addMeleeWeapon(&m_piercingClawsAlluress);
        addModel(reaperModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_piercingClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Daemonettes::Create(const ParameterList &parameters) {
        auto unit = new Daemonettes();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure(numModels, iconBearer, bannerBearer, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Daemonettes::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter( "Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Banner Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Daemonettes", factoryMethod);
        }
    }

    void Daemonettes::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Unit::computeBattleshockEffect(roll, numFled, numAdded);
        if (m_iconBearer) {
            // Icon Bearer
            if (roll == 1) {
                numAdded = Dice::RollD6();
            }
        }
    }

    void Daemonettes::restoreModels(int numModels) {
        // Icon Bearer
        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_piercingClaws);
            addModel(model);
        }
    }

    Rerolls Daemonettes::chargeRerolls() const {
        if (m_bannerBearer) {
            return Reroll_Failed;
        }
        return Unit::chargeRerolls();
    }

    int Daemonettes::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls Daemonettes::hornblowerBattleshockReroll(const Unit *unit) {
        if (!isFriendly(unit) && m_hornblower && (distanceTo(unit) <= 6.0)) return Reroll_Ones;

        return No_Rerolls;
    }

} // namespace Slaanesh