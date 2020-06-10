/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Quarrellers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Dispossessed {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool Quarrellers::s_registered = false;

    Quarrellers::Quarrellers() :
            Dispossessed("Quarrellers", 4, WOUNDS, 6, 5, false),
            m_duardinCrossbow(Weapon::Type::Missile, "Duardin Crossbow", 20, 1, 4, 4, 0, 1),
            m_duardinCrossbowVeteran(Weapon::Type::Missile, "Duardin Crossbow (Veteran)", 20, 1, 3, 4, 0, 1),
            m_rangersAxe(Weapon::Type::Melee, "Ranger's Axe", 1, 1, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, QUARRELLERS};
        m_weapons = {&m_duardinCrossbow, &m_duardinCrossbowVeteran, &m_rangersAxe};
    }

    bool Quarrellers::configure(int numModels, bool duardinBucklers, StandardOptions standard, bool drummer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_standard = standard;
        m_drummer = drummer;
        m_duardinBucklers = duardinBucklers;

        auto veteran = new Model(BASESIZE, wounds());
        veteran->addMissileWeapon(&m_duardinCrossbowVeteran);
        veteran->addMeleeWeapon(&m_rangersAxe);
        addModel(veteran);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_duardinCrossbow);
            model->addMeleeWeapon(&m_rangersAxe);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Quarrellers::Create(const ParameterList &parameters) {
        auto unit = new Quarrellers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool duardinBucklers = GetBoolParam("Duardin Bucklers", parameters, false);
        auto standard = (StandardOptions) GetEnumParam("Standard", parameters, None);
        bool drummer = GetBoolParam("Drummer", parameters, false);

        bool ok = unit->configure(numModels, duardinBucklers, standard, drummer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Quarrellers::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Quarrellers::None, Quarrellers::ClanBanner};
            static FactoryMethod factoryMethod = {
                    Quarrellers::Create,
                    Quarrellers::ValueToString,
                    Quarrellers::EnumStringToInt,
                    Quarrellers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Duardin Bucklers"),
                            EnumParameter("Standard", Quarrellers::None, weapons),
                            BoolParameter("Drummer"),
                            EnumParameter("Grudge", g_grudge[0], g_grudge)
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Quarrellers", factoryMethod);
        }
    }

    int Quarrellers::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Dispossessed::extraAttacks(attackingModel, weapon, target);
        if (weapon->name() == m_duardinCrossbow.name() && remainingModels() >= 20) {
            // Volley Fire
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && distanceTo(unit) > 3.0) {
                extra++;
            }
        }
        return extra;
    }

    Rerolls Quarrellers::toSaveRerolls(const Weapon *weapon) const {
        // Duardin Bucklers
        if (!m_ran && !m_charged) {
            if (!weapon->isMissile())
                return RerollFailed;
        }
        return Dispossessed::toSaveRerolls(weapon);
    }

    std::string Quarrellers::ValueToString(const Parameter &parameter) {
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

    int Quarrellers::EnumStringToInt(const std::string &enumString) {
        if (enumString == "None") {
            return None;
        } else if (enumString == "Runic Icon") {
            return RunicIcon;
        } else if (enumString == "Clan Banner") {
            return ClanBanner;
        }
        return Dispossessed::EnumStringToInt(enumString);
    }

    int Quarrellers::rollRunDistance() const {
        // Sound the Advance
        if (m_drummer) {
            return 4;
        }
        return Unit::rollRunDistance();
    }

    void Quarrellers::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Dispossessed::computeBattleshockEffect(roll, numFled, numAdded);

        if (m_standard == ClanBanner) {
            numFled = (numFled + 1) / 2;
        }
    }

    int Quarrellers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Dispossessed
