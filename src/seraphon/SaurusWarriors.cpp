/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusWarriors.h>
#include <UnitFactory.h>

namespace Seraphon {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    bool SaurusWarriors::s_registered = false;

    SaurusWarriors::SaurusWarriors() :
            SeraphonBase("Saurus Warriors", 5, WOUNDS, 8, 4, false),
            m_celestiteClub(Weapon::Type::Melee, "Celestite Club", 1, 1, 4, 3, -1, 1),
            m_celestiteClubAlpha(Weapon::Type::Melee, "Celestite Club", 1, 2, 4, 3, -1, 1),
            m_celestiteSpear(Weapon::Type::Melee, "Celestite Spear", 2, 1, 4, 3, 0, 1),
            m_celestiteSpearAlpha(Weapon::Type::Melee, "Celestite Spear", 2, 2, 4, 3, 0, 1),
            m_jaws(Weapon::Type::Melee, "Powerful Jaws", 1, 1, 5, 4, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SAURUS, SAURUS_WARRIORS};
        m_weapons = {&m_celestiteClub, &m_celestiteClubAlpha, &m_celestiteSpear, &m_celestiteSpearAlpha, &m_jaws};

        s_globalBraveryMod.connect(this, &SaurusWarriors::stardrakeIcon, &m_connection);
    }

    SaurusWarriors::~SaurusWarriors() {
        m_connection.disconnect();
    }

    bool SaurusWarriors::configure(int numModels, SaurusWarriors::WeaponOption weapons, bool iconBearer, bool wardrum) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_weaponOption = weapons;
        m_iconBearer = iconBearer;
        m_wardrum = wardrum;

        // Add the Alpha
        auto alpha = new Model(BASESIZE, wounds());
        if (m_weaponOption == CelestiteClub) {
            alpha->addMeleeWeapon(&m_celestiteClubAlpha);
        } else if (m_weaponOption == CelestiteSpear) {
            alpha->addMeleeWeapon(&m_celestiteSpearAlpha);
        }
        alpha->addMeleeWeapon(&m_jaws);
        addModel(alpha);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (m_weaponOption == CelestiteClub) {
                model->addMeleeWeapon(&m_celestiteClub);
            } else if (m_weaponOption == CelestiteSpear) {
                model->addMeleeWeapon(&m_celestiteSpear);
            }
            model->addMeleeWeapon(&m_jaws);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SaurusWarriors::Create(const ParameterList &parameters) {
        auto unit = new SaurusWarriors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, CelestiteClub);
        bool iconBearer = GetBoolParam("Stardrake Icon", parameters, false);
        bool wardrum = GetBoolParam("Wardrum", parameters, false);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, SeraphonBase::None);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure(numModels, weapons, iconBearer, wardrum);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string SaurusWarriors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == CelestiteClub) { return "Celestite Club"; }
            else if (parameter.intValue == CelestiteSpear) { return "Celestite Spear"; }
        }
        return SeraphonBase::ValueToString(parameter);
    }

    int SaurusWarriors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Celestite Club") { return CelestiteClub; }
        else if (enumString == "Celestite Spear") { return CelestiteSpear; }
        return SeraphonBase::EnumStringToInt(enumString);
    }

    void SaurusWarriors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Weapons", CelestiteClub, CelestiteClub, CelestiteSpear, 1},
                            {ParamType::Boolean, "Stardrake Icon", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Wardrum", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne,
                             SeraphonBase::Coalesced, 1},
                            {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None,
                             SeraphonBase::FangsOfSotek, 1}
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Saurus Warriors", factoryMethod);
        }
    }

    int SaurusWarriors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = SeraphonBase::extraAttacks(attackingModel, weapon, target);

        // Ordered Cohort
        if (((weapon->name() == m_celestiteSpear.name()) || (weapon->name() == m_celestiteClub.name())) &&
            (remainingModels() >= 15)) {
            extra++;
        }
        return extra;
    }

    int SaurusWarriors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int SaurusWarriors::stardrakeIcon(const Unit *target) {
        // Icon Bearer
        if (m_iconBearer && (target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 6.0)) {
            return -1;
        }

        return 0;
    }

    Rerolls SaurusWarriors::chargeRerolls() const {
        if (m_wardrum) return RerollFailed;

        return SeraphonBase::chargeRerolls();
    }

} //namespace Seraphon
