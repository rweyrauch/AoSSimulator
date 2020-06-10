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

namespace Slaanesh {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 300;


    bool Daemonettes::s_registered = false;

    Daemonettes::Daemonettes() :
            SlaaneshBase("Daemonettes", 6, WOUNDS, 10, 5, false),
            m_piercingClaws(Weapon::Type::Melee, "Piercing Claws", 1, 2, 4, 4, -1, 1),
            m_piercingClawsAlluress(Weapon::Type::Melee, "Piercing Claws", 1, 3, 4, 4, -1, 1) {
        m_keywords = {CHAOS, DAEMON, SLAANESH, HEDONITE, DAEMONETTES};
        m_weapons = {&m_piercingClaws, &m_piercingClawsAlluress};

        // Lithe and Swift
        m_runAndCharge = true;

        s_globalBattleshockReroll.connect(this, &Daemonettes::hornblowerBattleshockReroll, &m_hornblowerSlot);
    }

    Daemonettes::~Daemonettes() {
        m_hornblowerSlot.disconnect();
    }

    bool Daemonettes::configure(int numModels, bool iconBearer, bool bannerBearer, bool hornblower) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_bannerBearer = bannerBearer;
        m_hornblower = hornblower;

        // Add the Alluress
        auto reaperModel = new Model(BASESIZE, wounds());
        reaperModel->addMeleeWeapon(&m_piercingClawsAlluress);
        addModel(reaperModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_piercingClaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Daemonettes::Create(const ParameterList &parameters) {
        auto unit = new Daemonettes();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (Host) GetEnumParam("Host", parameters, Godseekers);
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
                            IntegerParameter( "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
                numAdded = Dice::rollD6();
            }
        }
    }

    void Daemonettes::restoreModels(int numModels) {
        // Icon Bearer
        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_piercingClaws);
            addModel(model);
        }
    }

    Rerolls Daemonettes::chargeRerolls() const {
        if (m_bannerBearer) {
            return RerollFailed;
        }
        return Unit::chargeRerolls();
    }

    int Daemonettes::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    Rerolls Daemonettes::hornblowerBattleshockReroll(const Unit *unit) {
        if (!isFriendly(unit) && m_hornblower && (distanceTo(unit) <= 6.0)) return RerollOnes;

        return NoRerolls;
    }

} // namespace Slaanesh