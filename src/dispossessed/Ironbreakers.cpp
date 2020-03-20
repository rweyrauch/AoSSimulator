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

namespace Dispossessed {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool Ironbreakers::s_registered = false;

    Ironbreakers::Ironbreakers() :
            Dispossessed("Ironbreakers", 4, WOUNDS, 7, 4, false),
            m_drakefirePistol(Weapon::Type::Missile, "Drakefire Pistol", 8, 1, 4, 3, -1, 1),
            m_drakefirePistolMelee(Weapon::Type::Melee, "Drakefire Pistol", 1, 1, 4, 4, 0, 1),
            m_axeOrHammer(Weapon::Type::Melee, "Ironbreaker Axe or Hammer", 1, 2, 3, 4, 0, 1),
            m_axeOrHammerIronbeard(Weapon::Type::Melee, "Ironbreaker Axe or Hammer", 1, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, IRONBREAKERS};
        m_weapons = {&m_drakefirePistol, &m_drakefirePistolMelee, &m_axeOrHammer, &m_axeOrHammerIronbeard};
    }

    bool Ironbreakers::configure(int numModels, WeaponOptions ironbeardWeapons, bool iconBearer, bool drummer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_iconBearer = iconBearer;
        m_drummer = drummer;

        auto ironbeard = new Model(BASESIZE, wounds());
        if (ironbeardWeapons == IronbreakerAxeOrHammer) {
            ironbeard->addMeleeWeapon(&m_axeOrHammerIronbeard);
        } else if (ironbeardWeapons == DrakefirePistolAndCinderblastBomb) {
            ironbeard->addMissileWeapon(&m_drakefirePistol);
            ironbeard->addMeleeWeapon(&m_drakefirePistolMelee);
            m_hasCinderblastBomb = true;
        } else if (ironbeardWeapons == PairedDrakefirePistols) {
            // two attacks when using dual-pistols
            ironbeard->addMissileWeapon(&m_drakefirePistol);
            ironbeard->addMissileWeapon(&m_drakefirePistol);
            ironbeard->addMeleeWeapon(&m_drakefirePistolMelee);
            ironbeard->addMeleeWeapon(&m_drakefirePistolMelee);
        }
        addModel(ironbeard);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_axeOrHammer);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Ironbreakers::Create(const ParameterList &parameters) {
        auto unit = new Ironbreakers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOptions weapon = (WeaponOptions) GetEnumParam("Ironbeard Weapon", parameters,
                                                            (int) IronbreakerAxeOrHammer);
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
            static FactoryMethod factoryMethod = {
                    Ironbreakers::Create,
                    Ironbreakers::ValueToString,
                    Ironbreakers::EnumStringToInt,
                    Ironbreakers::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {
                                    ParamType::Enum, "Ironbeard Weapon", Ironbreakers::IronbreakerAxeOrHammer,
                                    Ironbreakers::IronbreakerAxeOrHammer,
                                    Ironbreakers::PairedDrakefirePistols, 1
                            },
                            {ParamType::Boolean, "Icon Bearer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Boolean, "Drummer", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Enum, "Grudge", Dispossessed::StuckUp, Dispossessed::StuckUp,
                             Dispossessed::SneakyAmbushers, 1}
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
                return RerollFailed;
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
                int roll = Dice::rollD6();
                if (roll >= 2) {
                    units.front()->applyDamage({0, Dice::rollD3()});
                }
                m_hasCinderblastBomb = false;
            }
        }
    }

    std::string Ironbreakers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Ironbeard Weapon") {
            if (parameter.intValue == IronbreakerAxeOrHammer) {
                return "Ironbreaker Axe Or Hammer";
            } else if (parameter.intValue == DrakefirePistolAndCinderblastBomb) {
                return "Drakefire Pistol And Cinderblast Bomb";
            } else if (parameter.intValue == PairedDrakefirePistols) {
                return "Paired Drakefire Pistols";
            }
        }

        return Dispossessed::ValueToString(parameter);
    }

    int Ironbreakers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ironbreaker Axe Or Hammer") {
            return IronbreakerAxeOrHammer;
        } else if (enumString == "Drakefire Pistol And Cinderblast Bomb") {
            return DrakefirePistolAndCinderblastBomb;
        } else if (enumString == "Paired Drakefire Pistols") {
            return PairedDrakefirePistols;
        }
        return Dispossessed::EnumStringToInt(enumString);
    }

    int Ironbreakers::rollRunDistance() const {
        // Sound the Advance
        if (m_drummer) {
            return 4;
        }
        return Unit::rollRunDistance();
    }

    int Ironbreakers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Dispossessed
