/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Ironbreakers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "DispossessedPrivate.h"

namespace Dispossessed {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 360;

    bool Ironbreakers::s_registered = false;

    Ironbreakers::Ironbreakers() :
            Dispossessed("Ironbreakers", 4, g_wounds, 7, 4, false),
            m_drakefirePistol(Weapon::Type::Missile, "Drakefire Pistol", 8, 1, 4, 3, -1, 1),
            m_drakefirePistolMelee(Weapon::Type::Melee, "Drakefire Pistol", 1, 1, 4, 4, 0, 1),
            m_axeOrHammer(Weapon::Type::Melee, "Ironbreaker Axe or Hammer", 1, 2, 3, 4, 0, 1),
            m_axeOrHammerIronbeard(Weapon::Type::Melee, "Ironbreaker Axe or Hammer", 1, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, IRONBREAKERS};
        m_weapons = {&m_drakefirePistol, &m_drakefirePistolMelee, &m_axeOrHammer, &m_axeOrHammerIronbeard};
    }

    bool Ironbreakers::configure(int numModels, WeaponOptions ironbeardWeapons, bool iconBearer, bool drummer) {
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
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
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
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool drummer = GetBoolParam("Drummer", parameters, false);

        bool ok = unit->configure(numModels, weapon, iconBearer, drummer);
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
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Drummer"),
                            EnumParameter("Grudge", g_grudge[0], g_grudge)
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Ironbreakers", factoryMethod);
        }
    }

    Rerolls Ironbreakers::toSaveRerolls(const Weapon *weapon) const {
        // Gromril Shields
        if (!m_ran && !m_charged) {
            if (!weapon->isMissile())
                return Reroll_Failed;
        }
        return Dispossessed::toSaveRerolls(weapon);
    }

    int Ironbreakers::toSaveModifier(const Weapon *weapon) const {
        int modifier = Dispossessed::toSaveModifier(weapon);

        // Forge-proven Gromril Armour - ignore rend of less than -2 by cancelling it out.
        if (weapon->rend() == -1) {
            modifier = -weapon->rend();
        }

        return modifier;
    }

    void Ironbreakers::onStartShooting(PlayerId player) {
        Dispossessed::onStartShooting(player);

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

        return Dispossessed::ValueToString(parameter);
    }

    int Ironbreakers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ironbreaker Axe Or Hammer") {
            return Ironbreaker_Axe_Or_Hammer;
        } else if (enumString == "Drakefire Pistol And Cinderblast Bomb") {
            return Drakefire_Pistol_And_Cinderblast_Bomb;
        } else if (enumString == "Paired Drakefire Pistols") {
            return Paired_Drakefire_Pistols;
        }
        return Dispossessed::EnumStringToInt(enumString);
    }

    int Ironbreakers::rollRunDistance() const {
        // Sound the Advance
        if (isNamedModelAlive(Model::Drummer)) {
            return 4;
        }
        return Unit::rollRunDistance();
    }

    int Ironbreakers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Dispossessed
