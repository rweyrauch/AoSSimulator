/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Seekers.h>
#include <Weapon.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 150;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize/g_minUnitSize)*g_pointsPerBlock;

    bool Seekers::s_registered = false;

    Seekers::Seekers() :
            SlaaneshBase("Seekers", 14, g_wounds, 10, 5, false),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 2, 3, 4, -1, 1),
            m_piercingClawsHeartseeker(Weapon::Type::Melee, "Piercing Claws", 1, 3, 3, 4, -1, 1),
            m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTES, SLAANESH, HEDONITE, SEEKERS};
        m_weapons = {&m_piercingClaws, &m_piercingClawsHeartseeker, &m_poisonedTongue};
        m_hasMount = true;
        m_poisonedTongue.setMount(true);
        // Quicksilver Speed
        m_runAndCharge = true;

        s_globalBattleshockReroll.connect(this, &Seekers::hornblowerBattleshockReroll, &m_hornblowerSlot);
    }

    Seekers::~Seekers() {
        m_hornblowerSlot.disconnect();
    }

    bool Seekers::configure(int numModels, bool iconBearer, bool standardBearer, bool hornblower) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        // Add the Heartseeker
        auto reaperModel = new Model(g_basesize, wounds());
        reaperModel->addMeleeWeapon(&m_piercingClawsHeartseeker);
        reaperModel->addMeleeWeapon(&m_poisonedTongue);
        addModel(reaperModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_piercingClaws);
            model->addMeleeWeapon(&m_poisonedTongue);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            }
            else if (standardBearer) {
                model->setName(Model::BannerBearer);
                standardBearer = false;
            }
            else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Seekers::Create(const ParameterList &parameters) {
        auto unit = new Seekers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure(numModels, iconBearer, standardBearer, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Seekers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Seekers", factoryMethod);
        }
    }

    void Seekers::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Unit::computeBattleshockEffect(roll, numFled, numAdded);
        if (isNamedModelAlive(Model::IconBearer)) {
            // Icon Bearer
            if (roll == 1) {
                numAdded = Dice::RollD3();
            }
        }
    }

    void Seekers::restoreModels(int numModels) {
        // Icon Bearer
        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_piercingClaws);
            model->addMeleeWeapon(&m_poisonedTongue);
            addModel(model);
        }
    }

    Rerolls Seekers::chargeRerolls() const {
        if (isNamedModelAlive("Banner Bearer")) {
            return Rerolls::Failed;
        }
        return Unit::chargeRerolls();
    }

    int Seekers::runModifier() const {
        // Quicksilver Speed
        return Dice::RollD6();
    }

    int Seekers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls Seekers::hornblowerBattleshockReroll(const Unit *unit) {
        if (!isFriendly(unit) && isNamedModelAlive(Model::Hornblower) && (distanceTo(unit) <= 6.0)) return Rerolls::Ones;

        return Rerolls::None;
    }

} // namespace Slaanesh