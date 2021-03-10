/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Thunderers.h>
#include <UnitFactory.h>
#include <Board.h>
#include "DispossessedPrivate.h"

namespace Dispossessed {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 360;

    bool Thunderers::s_registered = false;

    Thunderers::Thunderers() :
            Dispossessed("Thunderers", 4, g_wounds, 6, 5, false),
            m_duardinHandgun(Weapon::Type::Missile, "Duardin Handgun", 16, 1, 4, 3, -1, 1),
            m_duardinHandgunVeteran(Weapon::Type::Missile, "Duardin Handgun (Veteran)", 16, 1, 3, 3, -1, 1),
            m_braceOfDuardinPistols(Weapon::Type::Missile, "Brace of Duardin Pistols", 8, 2, 4, 3, -1, 1),
            m_duardinHandgunMelee(Weapon::Type::Melee, "Duardin Handgun", 1, 1, 4, 5, 0, 1),
            m_braceOfDuardinPistolsMelee(Weapon::Type::Melee, "Brace of Duardin Pistols", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, THUNDERERS};
        m_weapons = {&m_duardinHandgun, &m_duardinHandgunVeteran, &m_braceOfDuardinPistols, &m_duardinHandgunMelee,
                     &m_braceOfDuardinPistolsMelee};
    }

    bool
    Thunderers::configure(int numModels, WeaponOptions veteranWeapon, bool duardinBucklers, StandardOptions standard,
                          bool drummers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_duardinBucklers = duardinBucklers;

        auto veteran = new Model(g_basesize, wounds());
        if (veteranWeapon == Duardin_Handgun) {
            veteran->addMissileWeapon(&m_duardinHandgunVeteran);
            veteran->addMeleeWeapon(&m_duardinHandgunMelee);
        } else if (veteranWeapon == Brace_Of_Duardin_Pistols) {
            veteran->addMissileWeapon(&m_braceOfDuardinPistols);
            veteran->addMeleeWeapon(&m_braceOfDuardinPistolsMelee);
        }
        addModel(veteran);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_duardinHandgun);
            model->addMeleeWeapon(&m_duardinHandgunMelee);
            if (standard == StandardOptions::Clan_Banner) {
                model->setName("Clan Banner");
                standard = StandardOptions::None;
            }
            else if (standard == StandardOptions::Runic_Icon) {
                model->setName("Runic Icon");
                standard = StandardOptions::None;
            }
            else if (drummers) {
                model->setName(Model::Drummer);
                drummers = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Thunderers::Create(const ParameterList &parameters) {
        auto unit = new Thunderers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapon = (WeaponOptions) GetEnumParam("Veteran Weapon", parameters, Duardin_Handgun);
        bool duardinBucklers = GetBoolParam("Duardin Bucklers", parameters, false);
        auto standard = (StandardOptions) GetEnumParam("Standard", parameters, None);
        bool drummer = GetBoolParam("Drummer", parameters, false);

        bool ok = unit->configure(numModels, weapon, duardinBucklers, standard, drummer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Thunderers::Init() {
        if (!s_registered) {
            static const std::array<int, 2> veteranWeapons = {Duardin_Handgun, Brace_Of_Duardin_Pistols};
            static const std::array<int, 3> weapons = {Thunderers::None, Thunderers::Runic_Icon, Thunderers::Clan_Banner};
            static FactoryMethod factoryMethod = {
                    Thunderers::Create,
                    Thunderers::ValueToString,
                    Thunderers::EnumStringToInt,
                    Thunderers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Veteran Weapon", Duardin_Handgun, veteranWeapons),
                            BoolParameter("Duardin Bucklers"),
                            EnumParameter("Standard", Thunderers::None, weapons),
                            BoolParameter("Drummers"),
                            EnumParameter("Grudge", g_grudge[0], g_grudge)
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Thunderers", factoryMethod);
        }
    }

    std::string Thunderers::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Veteran Weapon") {
            if (parameter.intValue == Duardin_Handgun) {
                return "Duardin Handgun";
            } else if (parameter.intValue == Brace_Of_Duardin_Pistols) {
                return "Brace Of Duardin Pistols";
            }
        }
        if (std::string(parameter.name) == "Standard") {
            if (parameter.intValue == None) {
                return "None";
            } else if (parameter.intValue == Runic_Icon) {
                return "Runic Icon";
            } else if (parameter.intValue == Clan_Banner) {
                return "Clan Banner";
            }
        }

        return Dispossessed::ValueToString(parameter);
    }

    int Thunderers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "None") {
            return None;
        } else if (enumString == "Runic Icon") {
            return Runic_Icon;
        } else if (enumString == "Clan Banner") {
            return Clan_Banner;
        } else if (enumString == "Duardin Handgun") {
            return Duardin_Handgun;
        } else if (enumString == "Brace Of Duardin Pistols") {
            return Brace_Of_Duardin_Pistols;
        }
        return Dispossessed::EnumStringToInt(enumString);
    }

    int Thunderers::rollRunDistance() {
        // Sound the Advance
        if (isNamedModelAlive(Model::Drummer)) {
            return 4;
        }
        return Unit::rollRunDistance();
    }

    void Thunderers::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Dispossessed::computeBattleshockEffect(roll, numFled, numAdded);

        if (isNamedModelAlive("Clan Banner")) {
            numFled = (numFled + 1) / 2;
        }
    }

    int Thunderers::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto modifier = Unit::toHitModifier(weapon, target);

        // Precision Fire
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        if ((remainingModels() >= 20) && units.empty()) {
            modifier += 1;
        }
        return modifier;
    }

    int Thunderers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Dispossessed
