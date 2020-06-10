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

namespace Dispossessed {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool Thunderers::s_registered = false;

    Thunderers::Thunderers() :
            Dispossessed("Thunderers", 4, WOUNDS, 6, 5, false),
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
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_standard = standard;
        m_drummers = drummers;
        m_duardinBucklers = duardinBucklers;

        auto veteran = new Model(BASESIZE, wounds());
        if (veteranWeapon == DuardinHandgun) {
            veteran->addMissileWeapon(&m_duardinHandgunVeteran);
            veteran->addMeleeWeapon(&m_duardinHandgunMelee);
        } else if (veteranWeapon == BraceOfDuardinPistols) {
            veteran->addMissileWeapon(&m_braceOfDuardinPistols);
            veteran->addMeleeWeapon(&m_braceOfDuardinPistolsMelee);
        }
        addModel(veteran);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_duardinHandgun);
            model->addMeleeWeapon(&m_duardinHandgunMelee);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Thunderers::Create(const ParameterList &parameters) {
        auto unit = new Thunderers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto weapon = (WeaponOptions) GetEnumParam("Veteran Weapon", parameters, DuardinHandgun);
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
            static const std::array<int, 2> veteranWeapons = {DuardinHandgun, BraceOfDuardinPistols};
            static const std::array<int, 3> weapons = {Thunderers::None, Thunderers::RunicIcon, Thunderers::ClanBanner};
            static FactoryMethod factoryMethod = {
                    Thunderers::Create,
                    Thunderers::ValueToString,
                    Thunderers::EnumStringToInt,
                    Thunderers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Veteran Weapon", DuardinHandgun, veteranWeapons),
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
            if (parameter.intValue == DuardinHandgun) {
                return "Duardin Handgun";
            } else if (parameter.intValue == BraceOfDuardinPistols) {
                return "Brace Of Duardin Pistols";
            }
        }
        if (std::string(parameter.name) == "Standard") {
            if (parameter.intValue == None) {
                return "None";
            } else if (parameter.intValue == RunicIcon) {
                return "Runic Icon";
            } else if (parameter.intValue == ClanBanner) {
                return "Clan Banner";
            }
        }

        return Dispossessed::ValueToString(parameter);
    }

    int Thunderers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "None") {
            return None;
        } else if (enumString == "Runic Icon") {
            return RunicIcon;
        } else if (enumString == "Clan Banner") {
            return ClanBanner;
        } else if (enumString == "Duardin Handgun") {
            return DuardinHandgun;
        } else if (enumString == "Brace Of Duardin Pistols") {
            return BraceOfDuardinPistols;
        }
        return Dispossessed::EnumStringToInt(enumString);
    }

    int Thunderers::rollRunDistance() const {
        // Sound the Advance
        if (m_drummers) {
            return 4;
        }
        return Unit::rollRunDistance();
    }

    void Thunderers::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Dispossessed::computeBattleshockEffect(roll, numFled, numAdded);

        if (m_standard == ClanBanner) {
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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Dispossessed
