/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/Orruks.h>
#include <UnitFactory.h>
#include <Board.h>
#include <array>

namespace Greenskinz {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 280;

    bool Orruks::s_registered = false;

    Orruks::Orruks() :
            Unit("Orruks", 5, g_wounds, 5, 5, false),
            m_orrukBows(Weapon::Type::Missile, "Orruk Bows", 18, 1, 5, 4, 0, 1),
            m_choppa(Weapon::Type::Melee, "Choppa", 1, 1, 4, 4, -1, 1),
            m_pigstikkaSpear(Weapon::Type::Melee, "Pigstikka Spear", 2, 1, 4, 4, 0, 1),
            m_cutta(Weapon::Type::Melee, "Cutta", 1, 1, 4, 5, 0, 1),
            m_orrukBowBoss(Weapon::Type::Missile, "Orruk Bow", 18, 2, 5, 4, 0, 1),
            m_choppaBoss(Weapon::Type::Melee, "Choppa", 1, 2, 4, 4, -1, 1),
            m_pigstikkaSpearBoss(Weapon::Type::Melee, "Pigstikka Spear", 2, 2, 4, 4, 0, 1),
            m_cuttaBoss(Weapon::Type::Melee, "Cutta", 1, 2, 4, 5, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, ORRUKS};
        m_weapons = {&m_orrukBows,
                     &m_choppa,
                     &m_pigstikkaSpear,
                     &m_cutta,
                     &m_orrukBowBoss,
                     &m_choppaBoss,
                     &m_pigstikkaSpearBoss,
                     &m_cuttaBoss};
    }

    bool Orruks::configure(int numModels, WeaponOption weapons, bool drummer, StandardOption standardBearer) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        m_weaponOption = weapons;

        // Add the boss
        auto boss = new Model(g_basesize, wounds());
        switch (weapons) {
            case Choppa_And_Shield:
                boss->addMeleeWeapon(&m_choppaBoss);
                break;
            case Spear_And_Shield:
                boss->addMeleeWeapon(&m_pigstikkaSpearBoss);
                break;
            case Paired_Choppas:
                boss->addMeleeWeapon(&m_choppaBoss);
                break;
            case Orruk_Bow_And_Cutta:
                boss->addMissileWeapon(&m_orrukBowBoss);
                boss->addMeleeWeapon(&m_choppaBoss);
                break;
        }
        addModel(boss);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            switch (weapons) {
                case Choppa_And_Shield:
                    model->addMeleeWeapon(&m_choppa);
                    break;
                case Spear_And_Shield:
                    model->addMeleeWeapon(&m_pigstikkaSpear);
                    break;
                case Paired_Choppas:
                    model->addMeleeWeapon(&m_choppa);
                    break;
                case Orruk_Bow_And_Cutta:
                    model->addMissileWeapon(&m_orrukBows);
                    model->addMeleeWeapon(&m_choppa);
                    break;
            }
            if (standardBearer != StandardOption::None) {
                if (standardBearer == StandardOption::Orruk_Banner)
                    model->setName("Orruk Banner");
                else if (standardBearer == StandardOption::Skull_Icon)
                    model->setName("Skull Icon");
                standardBearer = StandardOption::None;
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

    Unit *Orruks::Create(const ParameterList &parameters) {
        auto unit = new Orruks();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Choppa_And_Shield);
        bool drummer = GetBoolParam("Waaagh! Drummer", parameters, false);
        auto standardBearer = (StandardOption) GetEnumParam("Standard Bearer", parameters, Orruk_Banner);

        bool ok = unit->configure(numModels, weapons, drummer, standardBearer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Orruks::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {Choppa_And_Shield, Spear_And_Shield, Paired_Choppas, Orruk_Bow_And_Cutta};
            static const std::array<int, 3> banners = {StandardOption::None, StandardOption::Orruk_Banner, StandardOption::Skull_Icon};

            static FactoryMethod factoryMethod {
                    Orruks::Create,
                    Orruks::ValueToString,
                    Orruks::EnumStringToInt,
                    Orruks::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", weapons[0], weapons),
                            BoolParameter("Waaagh! Drummer"),
                            EnumParameter("Standard Bearer", banners[0], banners),
                    },
                    DESTRUCTION,
                    {GREENSKINZ}
            };
            s_registered = UnitFactory::Register("Orruks", factoryMethod);
        }
    }

    std::string Orruks::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Choppa_And_Shield) {
                return "Choppa and Shield";
            } else if (parameter.intValue == Spear_And_Shield) {
                return "Pigstikka Spear And Shield";
            } else if (parameter.intValue == Paired_Choppas) {
                return "Paired Choppas";
            } else if (parameter.intValue == Orruk_Bow_And_Cutta) {
                return "Orruk Bow and Cutta";
            }
        } else if (std::string(parameter.name) == "Standard Bearer") {
            if (parameter.intValue == Orruk_Banner) {
                return "Orruk Banner";
            } else if (parameter.intValue == Skull_Icon) {
                return "Skull Icon";
            } else if (parameter.intValue == None) {
                return "None";
            }
        }
        return ParameterValueToString(parameter);
    }

    int Orruks::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Choppa and Shield") {
            return Choppa_And_Shield;
        } else if (enumString == "Pigstikka Spear and Shield") {
            return Spear_And_Shield;
        } else if (enumString == "Paired Choppas") {
            return Paired_Choppas;
        } else if (enumString == "Orruk Bow and Cutta") {
            return Orruk_Bow_And_Cutta;
        } else if (enumString == "Orruk Banner") {
            return Orruk_Banner;
        } else if (enumString == "Skull Icon") {
            return Skull_Icon;
        } else if (enumString == "None") {
            return None;
        }
        return 0;
    }

    int Orruks::chargeModifier() const {
        auto modifier = Unit::chargeModifier();
        if (isNamedModelAlive(Model::Drummer))
            modifier += 2;
        return modifier;
    }

    int Orruks::braveryModifier() const {
        auto modifier = Unit::braveryModifier();
        if (isNamedModelAlive("Orruk Banner")) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
            if (!units.empty()) {
                modifier += 2;
            }
        }
        return modifier;
    }

    Rerolls Orruks::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Choppas
        if ((m_weaponOption == Paired_Choppas) && (weapon->name() == m_choppa.name())) {
            return Rerolls::Ones;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int Orruks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);
        // Mob Rule
        if ((remainingModels() >= 20) && !weapon->isMissile()) {
            attacks += 1;
        }
        return attacks;
    }

    Rerolls Orruks::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        // Waaagh! Shield
        if (!weapon->isMissile() && (m_weaponOption == Choppa_And_Shield || m_weaponOption == Spear_And_Shield)) {
            return Rerolls::Failed;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int Orruks::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto modifier = Unit::toHitModifier(weapon, target);

        // Ready Boyz! Aim! Fire!
        if (weapon->name() == m_orrukBows.name()) {
            if (!m_moved) {
                auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
                if (units.empty()) {
                    modifier += 1;
                }
            }
        }
        return modifier;
    }

    int Orruks::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Greenskinz
