/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Irondrakes.h>
#include <UnitFactory.h>
#include <Board.h>
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 450;

    bool Irondrakes::s_registered = false;

    Irondrakes::Irondrakes() :
            CitizenOfSigmar("Irondrakes", 4, WOUNDS, 7, 4, false),
            m_drakegun(Weapon::Type::Missile, "Drakegun", 16, 1, 3, 3, -1, 1),
            m_drakegunWarden(Weapon::Type::Missile, "Drakegun", 16, 1, 2, 3, -1, 1),
            m_grudgehammerTorpedo(Weapon::Type::Missile, "Grudgehammer Torpedo", 20, 1, 3, 3, -2, RAND_D3),
            m_drakefirePistol(Weapon::Type::Missile, "Drakefire Pistol", 8, 1, 4, 3, -1, 1),
            m_drakefirePistolMelee(Weapon::Type::Melee, "Drakefire Pistol", 1, 1, 4, 4, 0, 1),
            m_mailedFist(Weapon::Type::Melee, "Mailed Fist", 1, 1, 4, 5, 0, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, IRONDRAKES};
        m_weapons = {&m_drakegun, &m_drakegunWarden, &m_grudgehammerTorpedo, &m_drakefirePistol,
                     &m_drakefirePistolMelee, &m_mailedFist};
    }

    bool Irondrakes::configure(int numModels, WeaponOptions ironWardenWeapons, bool standardBearer, bool hornblower) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_standardBearer = standardBearer;
        m_hornblower = hornblower;

        auto ironwarden = new Model(BASESIZE, wounds());
        if (ironWardenWeapons == Drakegun) {
            ironwarden->addMissileWeapon(&m_drakegunWarden);
            ironwarden->addMeleeWeapon(&m_mailedFist);
        } else if (ironWardenWeapons == DrakefirePistolAndCinderblastBomb) {
            ironwarden->addMissileWeapon(&m_drakefirePistol);
            ironwarden->addMeleeWeapon(&m_drakefirePistolMelee);
            m_hasCinderblastBomb = true;
        } else if (ironWardenWeapons == PairedDrakefirePistols) {
            // two attacks when using dual-pistols
            ironwarden->addMissileWeapon(&m_drakefirePistol);
            ironwarden->addMissileWeapon(&m_drakefirePistol);
            ironwarden->addMeleeWeapon(&m_drakefirePistolMelee);
            ironwarden->addMeleeWeapon(&m_drakefirePistolMelee);
        } else if (ironWardenWeapons == GrudgehammerTorpedo) {
            ironwarden->addMissileWeapon(&m_grudgehammerTorpedo);
            ironwarden->addMeleeWeapon(&m_mailedFist);
        }
        addModel(ironwarden);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_drakegun);
            model->addMeleeWeapon(&m_mailedFist);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Irondrakes::Create(const ParameterList &parameters) {
        auto unit = new Irondrakes();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOptions weapon = (WeaponOptions) GetEnumParam("Ironwarden Weapon", parameters, (int) Drakegun);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, weapon, standardBearer, hornblower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Irondrakes::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {Drakegun, GrudgehammerTorpedo, DrakefirePistolAndCinderblastBomb, PairedDrakefirePistols};
            static FactoryMethod factoryMethod = {
                    Irondrakes::Create,
                    Irondrakes::ValueToString,
                    Irondrakes::EnumStringToInt,
                    Irondrakes::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Ironwarden Weapon", Drakegun, weapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Irondrakes", factoryMethod);
        }
    }

    int Irondrakes::toSaveModifier(const Weapon *weapon) const {
        int modifier = CitizenOfSigmar::toSaveModifier(weapon);

        // Forge-proven Gromril Armour - ignore rend of less than -2 by cancelling it out.
        if (weapon->rend() == -1) {
            modifier = -weapon->rend();
        }

        return modifier;
    }

    void Irondrakes::onStartShooting(PlayerId player) {
        CitizenOfSigmar::onStartShooting(player);

        // Cinderblast Bomb
        if (m_hasCinderblastBomb) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6);
            if (!units.empty()) {
                int roll = Dice::rollD6();
                if (roll >= 2) {
                    units.front()->applyDamage({0, Dice::rollD3()});
                }
                m_hasCinderblastBomb = false;
            }
        }
    }

    Wounds Irondrakes::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Grudgehammer Torpedo
        if (weapon->name() == m_grudgehammerTorpedo.name() && target->hasKeyword(MONSTER)) {
            return {Dice::rollD6(), 0};
        }
        return CitizenOfSigmar::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Irondrakes::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = CitizenOfSigmar::extraAttacks(attackingModel, weapon, target);
        if (weapon->name() == m_drakegun.name() && !m_moved) {
            // Blaze Away
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && distanceTo(unit) > 3.0) {
                extra++;
            }
        }
        return extra;
    }

    std::string Irondrakes::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Ironwarden Weapon") {
            if (parameter.intValue == Drakegun) {
                return "Drakegun";
            } else if (parameter.intValue == GrudgehammerTorpedo) {
                return "Grudgehammer Torpedo";
            } else if (parameter.intValue == DrakefirePistolAndCinderblastBomb) {
                return "Drakefire Pistol And Cinderblast Bomb";
            } else if (parameter.intValue == PairedDrakefirePistols) {
                return "Paired Drakefire Pistols";
            }
        }

        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Irondrakes::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Drakegun") {
            return Drakegun;
        } else if (enumString == "Grudgehammer Torpedo") {
            return GrudgehammerTorpedo;
        } else if (enumString == "Drakefire Pistol And CinderblastBomb") {
            return DrakefirePistolAndCinderblastBomb;
        } else if (enumString == "Paired Drakefire Pistols") {
            return PairedDrakefirePistols;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    int Irondrakes::runModifier() const {
        auto mod = Unit::runModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int Irondrakes::chargeModifier() const {
        auto mod = Unit::chargeModifier();
        if (m_hornblower) mod++;
        return mod;
    }

    int Irondrakes::braveryModifier() const {
        auto mod = Unit::braveryModifier();
        if (m_standardBearer) mod++;
        return mod;
    }

    int Irondrakes::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace CitiesOfSigmar
