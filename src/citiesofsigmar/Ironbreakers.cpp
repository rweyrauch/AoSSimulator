/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Ironbreakers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 270;

    bool Ironbreakers::s_registered = false;

    Ironbreakers::Ironbreakers() :
            CitizenOfSigmar("Ironbreakers", 4, g_wounds, 7, 4, false),
            m_drakefirePistol(Weapon::Type::Missile, "Drakefire Pistol", 8, 1, 4, 3, -1, 1),
            m_drakefirePistolMelee(Weapon::Type::Melee, "Drakefire Pistol", 1, 1, 4, 4, 0, 1),
            m_axeOrHammer(Weapon::Type::Melee, "Ironbreaker Axe or Hammer", 1, 2, 3, 4, 0, 1),
            m_axeOrHammerIronbeard(Weapon::Type::Melee, "Ironbreaker Axe or Hammer", 1, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, IRONBREAKERS};
        m_weapons = {&m_drakefirePistol, &m_drakefirePistolMelee, &m_axeOrHammer, &m_axeOrHammerIronbeard};
        m_battleFieldRole = Role::Battleline;
    }

    bool Ironbreakers::configure(int numModels, WeaponOptions ironbeardWeapons, bool standardBearer, bool drummer) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto ironbeard = new Model(g_basesize, wounds());
        if (ironbeardWeapons == Ironbreaker_Axe_Or_Hammer) {
            ironbeard->addMeleeWeapon(&m_axeOrHammerIronbeard);
        } else if (ironbeardWeapons == Drakefire_Pistol_And_Cinderblast_Bomb) {
            ironbeard->addMissileWeapon(&m_drakefirePistol);
            ironbeard->addMeleeWeapon(&m_drakefirePistolMelee);
            m_hasCinderblastBomb = true;
        } else if (ironbeardWeapons == Paired_Drakefire_Pistols) {
            // two attacks when using dual-pistols
            ironbeard->addMissileWeapon(&m_drakefirePistol);
            ironbeard->addMissileWeapon(&m_drakefirePistol);
            ironbeard->addMeleeWeapon(&m_drakefirePistolMelee);
            ironbeard->addMeleeWeapon(&m_drakefirePistolMelee);
        }
        addModel(ironbeard);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_axeOrHammer);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            }
            else if (drummer) {
                model->setName(Model::Drummer);
                drummer = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Ironbreakers::Create(const ParameterList &parameters) {
        auto unit = new Ironbreakers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOptions weapon = (WeaponOptions) GetEnumParam("Ironbeard Weapon", parameters,
                                                            (int) Ironbreaker_Axe_Or_Hammer);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool drummer = GetBoolParam("Drummer", parameters, false);

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        bool ok = unit->configure(numModels, weapon, standardBearer, drummer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Ironbreakers::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Ironbreaker_Axe_Or_Hammer, Drakefire_Pistol_And_Cinderblast_Bomb, Paired_Drakefire_Pistols};
            static FactoryMethod factoryMethod = {
                    Ironbreakers::Create,
                    Ironbreakers::ValueToString,
                    Ironbreakers::EnumStringToInt,
                    Ironbreakers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Ironbeard Weapon", Ironbreaker_Axe_Or_Hammer, weapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Drummer"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Ironbreakers", factoryMethod);
        }
    }

    Rerolls Ironbreakers::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Gromril Shields
        if (!m_ran && !m_charged) {
            if (!weapon->isMissile())
                return Rerolls::Failed;
        }
        return CitizenOfSigmar::toSaveRerolls(weapon, attacker);
    }

    int Ironbreakers::toSaveModifier(const Weapon *weapon, const Unit* attacker) const {
        int modifier = CitizenOfSigmar::toSaveModifier(weapon, attacker);

        // Forge-proven Gromril Armour - ignore rend of less than -2 by cancelling it out.
        if (weapon->rend() == -1) {
            modifier = -weapon->rend();
        }

        return modifier;
    }

    void Ironbreakers::onStartShooting(PlayerId player) {
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

    std::string Ironbreakers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Ironbeard Weapon") {
            if (parameter.intValue == Ironbreaker_Axe_Or_Hammer) {
                return "Ironbreaker Axe Or Hammer";
            } else if (parameter.intValue == Drakefire_Pistol_And_Cinderblast_Bomb) {
                return "Drakefire Pistol And Cinderblast Bomb";
            } else if (parameter.intValue == Paired_Drakefire_Pistols) {
                return "Paired Drakefire Pistols";
            }
        }

        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Ironbreakers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ironbreaker Axe Or Hammer") {
            return Ironbreaker_Axe_Or_Hammer;
        } else if (enumString == "Drakefire Pistol And Cinderblast Bomb") {
            return Drakefire_Pistol_And_Cinderblast_Bomb;
        } else if (enumString == "Paired Drakefire Pistols") {
            return Paired_Drakefire_Pistols;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    int Ironbreakers::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Drummer)) mod++;
        return mod;
    }

    int Ironbreakers::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Drummer)) mod++;
        return mod;
    }

    int Ironbreakers::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int Ironbreakers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar
