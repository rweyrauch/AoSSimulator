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

    Bloodcrushers::Bloodcrushers() :
            KhorneBase("Bloodcrushers", 8, g_wounds, 10, 4, false),
            m_hellblade(Weapon::Type::Melee, "Hellblade", 1, 1, 4, 3, -1, 1),
            m_hellbladeHunter(Weapon::Type::Melee, "Hellblade", 1, 2, 4, 3, -1, 1),
            m_brazenHooves(Weapon::Type::Melee, "Brazen Hooves", 1, 3, 3, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, BLOODLETTER, KHORNE, BLOODCRUSHERS};
        m_weapons = {&m_hellblade, &m_hellbladeHunter, &m_brazenHooves};
        m_hasMount = true;

        s_globalBattleshockReroll.connect(this, &Bloodcrushers::hornblowerBattleshockReroll, &m_hornblowerSlot);
    }

    Bloodcrushers::~Bloodcrushers() {
        m_hornblowerSlot.disconnect();
    }

    bool Bloodcrushers::configure(int numModels, bool iconBearer, bool hornblowers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

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
                model->setName("Icon Bearer");
                iconBearer = false;
            }
            else if (hornblowers) {
                model->setName("Hornblower");
                hornblowers = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Bloodcrushers::Create(const ParameterList &parameters) {
        auto unit = new Bloodcrushers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure(numModels, iconBearer, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

            SimLog(Verbosity::Narrative, "%s Murderous Charge inflicted %d mortal wounds on %s\n",
                   name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

            m_meleeTarget->applyDamage(wounds, this);
        }
        KhorneBase::onCharged();
    }

    Wounds Bloodcrushers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Bloodcrushers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Bloodcrushers::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        KhorneBase::computeBattleshockEffect(roll, numFled, numAdded);
        if (isNamedModelAlive("Icon Bearer")) {
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
        if (isNamedModelAlive("Hornblower") && !isFriendly(unit) && (distanceTo(unit) <= 8.0)) return Reroll_Ones;

        return No_Rerolls;
    }

} // namespace Khorne
