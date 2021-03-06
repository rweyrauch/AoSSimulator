/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodcrushers.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Bloodcrushers::s_registered = false;

    Bloodcrushers::Bloodcrushers(SlaughterHost host, int numModels, bool iconBearer, bool hornblowers, int points) :
            KhorneBase("Bloodcrushers", 8, g_wounds, 10, 4, false, points) {
        m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, BLOODCRUSHERS};
        m_weapons = {&m_hellblade, &m_hellbladeHunter, &m_brazenHooves};
        m_hasMount = true;
        m_brazenHooves.setMount(true);
        s_globalBattleshockReroll.connect(this, &Bloodcrushers::hornblowerBattleshockReroll, &m_hornblowerSlot);

        setSlaughterHost(host);

        // Add the Bloodhunter
        auto hunter = new Model(g_basesize, wounds());
        hunter->addMeleeWeapon(&m_hellbladeHunter);
        hunter->addMeleeWeapon(&m_brazenHooves);
        addModel(hunter);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_hellblade);
            model->addMeleeWeapon(&m_brazenHooves);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            } else if (hornblowers) {
                model->setName(Model::Hornblower);
                hornblowers = false;
            }
            addModel(model);
        }
    }

    Bloodcrushers::~Bloodcrushers() {
        m_hornblowerSlot.disconnect();
    }

    Unit *Bloodcrushers::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        return new Bloodcrushers(host, numModels, iconBearer, hornblowers, ComputePoints(parameters));
    }

    void Bloodcrushers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Bloodcrushers::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Bloodcrushers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodcrushers", factoryMethod);
        }
    }

    void Bloodcrushers::onCharged() {
        // Murderous Charge
        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            Dice::RollResult rolls;
            Dice::RollD6(remainingModels(), rolls);
            Wounds wounds = {0, 0};
            if (remainingModels() >= 6) {
                for (int i = 0; i < rolls.rollsGE(2); i++) {
                    wounds.mortal += Dice::RollD3();
                }
            } else {
                wounds.mortal = rolls.rollsGE(2);
            }

            PLOG_INFO.printf("%s Murderous Charge inflicted %d mortal wounds on %s\n",
                             name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

            m_meleeTarget->applyDamage(wounds, this);
        }
        KhorneBase::onCharged();
    }

    Wounds Bloodcrushers::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Bloodcrushers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Bloodcrushers::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        KhorneBase::computeBattleshockEffect(roll, numFled, numAdded);
        if (isNamedModelAlive(Model::IconBearer)) {
            // Icon Bearer
            numAdded = 1;
        }
    }

    void Bloodcrushers::restoreModels(int numModels) {
        // Icon Bearer
        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_hellblade);
            addModel(model);
        }
    }

    Rerolls Bloodcrushers::hornblowerBattleshockReroll(const Unit *unit) {
        if (isNamedModelAlive(Model::Hornblower) && !isFriendly(unit) && (distanceTo(unit) <= 8.0))
            return Rerolls::Ones;

        return Rerolls::None;
    }

} // namespace Khorne
