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
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 150;
    static const int g_pointsMaxUnitSize = 450;

    bool Irondrakes::s_registered = false;

    Irondrakes::Irondrakes(City city, int numModels, WeaponOptions ironWardenWeapons, bool standardBearer, bool hornblower, int points) :
            CitizenOfSigmar(city, "Irondrakes", 4, g_wounds, 7, 4, false, points),
            m_drakegun(Weapon::Type::Missile, "Drakegun", 16, 1, 3, 3, -1, 1),
            m_drakegunWarden(Weapon::Type::Missile, "Drakegun", 16, 1, 2, 3, -1, 1),
            m_grudgehammerTorpedo(Weapon::Type::Missile, "Grudgehammer Torpedo", 20, 1, 3, 3, -2, RAND_D3),
            m_drakefirePistol(Weapon::Type::Missile, "Drakefire Pistol", 8, 1, 4, 3, -1, 1),
            m_drakefirePistolMelee(Weapon::Type::Melee, "Drakefire Pistol", 1, 1, 4, 4, 0, 1),
            m_mailedFist(Weapon::Type::Melee, "Mailed Fist", 1, 1, 4, 5, 0, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, IRONDRAKES};
        m_weapons = {&m_drakegun, &m_drakegunWarden, &m_grudgehammerTorpedo, &m_drakefirePistol,
                     &m_drakefirePistolMelee, &m_mailedFist};

        auto ironwarden = new Model(g_basesize, wounds());
        if (ironWardenWeapons == Drakegun) {
            ironwarden->addMissileWeapon(&m_drakegunWarden);
            ironwarden->addMeleeWeapon(&m_mailedFist);
        } else if (ironWardenWeapons == Drakefire_Pistol_And_Cinderblast_Bomb) {
            ironwarden->addMissileWeapon(&m_drakefirePistol);
            ironwarden->addMeleeWeapon(&m_drakefirePistolMelee);
            m_hasCinderblastBomb = true;
        } else if (ironWardenWeapons == Paired_Drakefire_Pistols) {
            // two attacks when using dual-pistols
            ironwarden->addMissileWeapon(&m_drakefirePistol);
            ironwarden->addMissileWeapon(&m_drakefirePistol);
            ironwarden->addMeleeWeapon(&m_drakefirePistolMelee);
            ironwarden->addMeleeWeapon(&m_drakefirePistolMelee);
        } else if (ironWardenWeapons == Grudgehammer_Torpedo) {
            ironwarden->addMissileWeapon(&m_grudgehammerTorpedo);
            ironwarden->addMeleeWeapon(&m_mailedFist);
        }
        addModel(ironwarden);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_drakegun);
            model->addMeleeWeapon(&m_mailedFist);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            addModel(model);
        }
    }

    Unit *Irondrakes::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapon = (WeaponOptions) GetEnumParam("Ironwarden Weapon", parameters, (int) Drakegun);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);
        return new Irondrakes(city, numModels, weapon, standardBearer, hornblower, ComputePoints(parameters));
    }

    void Irondrakes::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {Drakegun, Grudgehammer_Torpedo,
                                                       Drakefire_Pistol_And_Cinderblast_Bomb, Paired_Drakefire_Pistols};
            static FactoryMethod factoryMethod = {
                    Irondrakes::Create,
                    Irondrakes::ValueToString,
                    Irondrakes::EnumStringToInt,
                    Irondrakes::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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

    int Irondrakes::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = CitizenOfSigmar::toSaveModifier(weapon, attacker);

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
                int roll = Dice::RollD6();
                if (roll >= 2) {
                    units.front()->applyDamage({0, Dice::RollD3()}, this);
                }
                m_hasCinderblastBomb = false;
            }
        }
    }

    Wounds Irondrakes::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Grudgehammer Torpedo
        if (weapon->name() == m_grudgehammerTorpedo.name() && target->hasKeyword(MONSTER)) {
            return {Dice::RollD6(), 0};
        }
        return CitizenOfSigmar::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
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
            } else if (parameter.intValue == Grudgehammer_Torpedo) {
                return "Grudgehammer Torpedo";
            } else if (parameter.intValue == Drakefire_Pistol_And_Cinderblast_Bomb) {
                return "Drakefire Pistol And Cinderblast Bomb";
            } else if (parameter.intValue == Paired_Drakefire_Pistols) {
                return "Paired Drakefire Pistols";
            }
        }

        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Irondrakes::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Drakegun") {
            return Drakegun;
        } else if (enumString == "Grudgehammer Torpedo") {
            return Grudgehammer_Torpedo;
        } else if (enumString == "Drakefire Pistol And CinderblastBomb") {
            return Drakefire_Pistol_And_Cinderblast_Bomb;
        } else if (enumString == "Paired Drakefire Pistols") {
            return Paired_Drakefire_Pistols;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    int Irondrakes::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int Irondrakes::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int Irondrakes::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int Irondrakes::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar
