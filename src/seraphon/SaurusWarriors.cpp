/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusWarriors.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 320;

    bool SaurusWarriors::s_registered = false;

    SaurusWarriors::SaurusWarriors(WayOfTheSeraphon way, Constellation constellation, int numModels, WeaponOption weapons, bool iconBearer, bool wardrum) :
            SeraphonBase("Saurus Warriors", 5, g_wounds, 8, 4, false) {
        m_keywords = {ORDER, SERAPHON, SAURUS, SAURUS_WARRIORS};
        m_weapons = {&m_celestiteClub, &m_celestiteClubAlpha, &m_celestiteSpear, &m_celestiteSpearAlpha, &m_jaws};
        m_battleFieldRole = Role::Battleline;

        s_globalBraveryMod.connect(this, &SaurusWarriors::stardrakeIcon, &m_connection);

        setWayOfTheSeraphon(way, constellation);

        m_weaponOption = weapons;

        // Add the Alpha
        auto alpha = new Model(g_basesize, wounds());
        if (m_weaponOption == Celestite_Club) {
            alpha->addMeleeWeapon(&m_celestiteClubAlpha);
        } else if (m_weaponOption == Celestite_Spear) {
            alpha->addMeleeWeapon(&m_celestiteSpearAlpha);
        }
        alpha->addMeleeWeapon(&m_jaws);
        addModel(alpha);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (m_weaponOption == Celestite_Club) {
                model->addMeleeWeapon(&m_celestiteClub);
            } else if (m_weaponOption == Celestite_Spear) {
                model->addMeleeWeapon(&m_celestiteSpear);
            }
            model->addMeleeWeapon(&m_jaws);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            } else if (wardrum) {
                model->setName("Wardrum");
                wardrum = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    SaurusWarriors::~SaurusWarriors() {
        m_connection.disconnect();
    }

    Unit *SaurusWarriors::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Celestite_Club);
        bool iconBearer = GetBoolParam("Stardrake Icon", parameters, false);
        bool wardrum = GetBoolParam("Wardrum", parameters, false);
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);

        return new SaurusWarriors(way, constellation, numModels, weapons, iconBearer, wardrum);
    }

    std::string SaurusWarriors::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Celestite_Club) { return "Celestite Club"; }
            else if (parameter.intValue == Celestite_Spear) { return "Celestite Spear"; }
        }
        return SeraphonBase::ValueToString(parameter);
    }

    int SaurusWarriors::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Celestite Club") { return Celestite_Club; }
        else if (enumString == "Celestite Spear") { return Celestite_Spear; }
        return SeraphonBase::EnumStringToInt(enumString);
    }

    void SaurusWarriors::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Celestite_Club, Celestite_Spear};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Celestite_Club, weapons),
                            BoolParameter("Stardrake Icon"),
                            BoolParameter("Wardrum"),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
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
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int SaurusWarriors::stardrakeIcon(const Unit *target) {
        // Icon Bearer
        if (isNamedModelAlive(Model::IconBearer) && (target->owningPlayer() != owningPlayer()) &&
            (distanceTo(target) <= 6.0)) {
            return -1;
        }

        return 0;
    }

    Rerolls SaurusWarriors::chargeRerolls() const {
        if (isNamedModelAlive("Wardrum")) return Rerolls::Failed;

        return SeraphonBase::chargeRerolls();
    }

} //namespace Seraphon
