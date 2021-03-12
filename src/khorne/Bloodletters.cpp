/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <algorithm>
#include <khorne/Bloodletters.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 300;

    bool Bloodletters::s_registered = false;

    Bloodletters::Bloodletters() :
            KhorneBase("Bloodletters", 5, g_wounds, 10, 5, false),
            m_hellblade(Weapon::Type::Melee, "Hellblade", 1, 1, 4, 3, -1, 1),
            m_hellbladeReaper(Weapon::Type::Melee, "Hellblade", 1, 2, 4, 3, -1, 1) {
        m_keywords = {CHAOS, DAEMON, KHORNE, BLOODLETTERS};
        m_weapons = {&m_hellblade, &m_hellbladeReaper};
        m_battleFieldRole = Role::Battleline;
        s_globalBattleshockReroll.connect(this, &Bloodletters::hornblowerBattleshockReroll, &m_hornblowerSlot);
    }

    Bloodletters::~Bloodletters() {
        m_hornblowerSlot.disconnect();
    }

    bool Bloodletters::configure(int numModels, bool iconBearer, bool standardBearer, bool hornblowers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        // Add the Hellreaper
        auto reaperModel = new Model(g_basesize, wounds());
        reaperModel->addMeleeWeapon(&m_hellbladeReaper);
        addModel(reaperModel);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_hellblade);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            } else if (hornblowers) {
                model->setName(Model::Hornblower);
                hornblowers = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds Bloodletters::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Decapitating Blow
        if (hitRoll == 6) {
            return {weapon->damage(), 1};
        }
        return KhorneBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Unit *Bloodletters::Create(const ParameterList &parameters) {
        auto unit = new Bloodletters();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure(numModels, iconBearer, standardBearer, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Bloodletters::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Bloodletters::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Bloodletters::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodletters", factoryMethod);
        }
    }

    int Bloodletters::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // Murderous Tide
        int attacks = KhorneBase::extraAttacks(attackingModel, weapon, target);
        if (remainingModels() >= 20) {
            attacks += 1;
        }
        return attacks;
    }

    Rerolls Bloodletters::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Locus of Fury
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
        for (auto ip : units) {
            if (ip->hasKeyword(DAEMON) && ip->hasKeyword(KHORNE) && ip->hasKeyword(HERO)) {
                return Rerolls::Ones;
            }
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    void Bloodletters::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        KhorneBase::computeBattleshockEffect(roll, numFled, numAdded);
        if (isNamedModelAlive(Model::IconBearer)) {
            // Icon Bearer
            if (roll == 1) {
                numAdded = Dice::RollD6();
            }
        }
    }

    void Bloodletters::restoreModels(int numModels) {
        // Icon Bearer
        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_hellblade);
            addModel(model);
        }
    }

    int Bloodletters::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls Bloodletters::chargeRerolls() const {
        if (isNamedModelAlive(Model::StandardBearer)) return Rerolls::Failed;
        return KhorneBase::chargeRerolls();
    }

    Rerolls Bloodletters::hornblowerBattleshockReroll(const Unit *unit) {
        if (isNamedModelAlive(Model::Hornblower) && !isFriendly(unit) && (distanceTo(unit) <= 8.0))
            return Rerolls::Ones;

        return Rerolls::None;
    }

} // namespace Khorne