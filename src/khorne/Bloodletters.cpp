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

namespace Khorne {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 110;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    bool Bloodletters::s_registered = false;

    Bloodletters::Bloodletters() :
            KhorneBase("Bloodletters", 5, WOUNDS, 10, 5, false),
            m_hellblade(Weapon::Type::Melee, "Hellblade", 1, 1, 4, 3, -1, 1),
            m_hellbladeReaper(Weapon::Type::Melee, "Hellblade", 1, 2, 4, 3, -1, 1) {
        m_keywords = {CHAOS, DAEMON, KHORNE, BLOODLETTERS};
        m_weapons = {&m_hellblade, &m_hellbladeReaper};

        s_globalBattleshockReroll.connect(this, &Bloodletters::hornblowerBattleshockReroll, &m_hornblowerSlot);
    }

    Bloodletters::~Bloodletters() {
        m_hornblowerSlot.disconnect();
    }

    bool Bloodletters::configure(int numModels, bool iconBearer, bool standardBearer, bool hornblowers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_standarBearer = standardBearer;
        m_hornblower = hornblowers;

        // Add the Hellreaper
        auto reaperModel = new Model(BASESIZE, wounds());
        reaperModel->addMeleeWeapon(&m_hellbladeReaper);
        addModel(reaperModel);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_hellblade);
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
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, Khorne::None);
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
                return RerollOnes;
            }
        }
        return KhorneBase::toHitRerolls(weapon, target);
    }

    void Bloodletters::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        KhorneBase::computeBattleshockEffect(roll, numFled, numAdded);
        if (m_iconBearer) {
            // Icon Bearer
            if (roll == 1) {
                numAdded = Dice::rollD6();
            }
        }
    }

    void Bloodletters::restoreModels(int numModels) {
        // Icon Bearer
        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_hellblade);
            addModel(model);
        }
    }

    int Bloodletters::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    Rerolls Bloodletters::chargeRerolls() const {
        if (m_standarBearer) return RerollFailed;
        return Unit::chargeRerolls();
    }

    Rerolls Bloodletters::hornblowerBattleshockReroll(const Unit *unit) {
        if (m_hornblower && !isFriendly(unit) && (distanceTo(unit) <= 8.0)) return RerollOnes;

        return NoRerolls;
    }

} // namespace Khorne