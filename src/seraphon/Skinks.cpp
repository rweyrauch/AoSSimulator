/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Skinks.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 240;

    bool Skinks::s_registered = false;

    Skinks::Skinks() :
            SeraphonBase("Skinks", 8, g_wounds, 5, 6, false),
            m_javelin(Weapon::Type::Missile, "Meteoric Javelin", 8, 1, 5, 4, 0, 1),
            m_boltspitter(Weapon::Type::Missile, "Boltspitter", 16, 1, 5, 5, 0, 1),
            m_dagger(Weapon::Type::Melee, "Celestite Dagger", 1, 1, 5, 5, 0, 1),
            m_daggerAlpha(Weapon::Type::Melee, "Celestite Dagger", 1, 2, 5, 5, 0, 1),
            m_club(Weapon::Type::Melee, "Moonstone Club", 1, 1, 4, 4, 0, 1),
            m_clubAlpha(Weapon::Type::Melee, "Moonstone Club", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, DAEMON, CELESTIAL, SERAPHON, SKINKS};
        m_weapons = {&m_javelin, &m_boltspitter, &m_dagger, &m_daggerAlpha, &m_club, &m_clubAlpha};
    }

    bool Skinks::configure(int numModels, WeaponOption weapons) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_weaponOption = weapons;

        // Add the Alpha
        auto alpha = new Model(g_basesize, wounds());
        switch (m_weaponOption) {
            case Javelins_Daggers_And_Bucklers:
                alpha->addMissileWeapon(&m_javelin);
                alpha->addMeleeWeapon(&m_daggerAlpha);
                break;
            case Boltspitters_Daggers_And_Bucklers:
                alpha->addMissileWeapon(&m_boltspitter);
                alpha->addMeleeWeapon(&m_daggerAlpha);
                break;
            case Boltspitters_And_Clubs:
                alpha->addMissileWeapon(&m_boltspitter);
                alpha->addMeleeWeapon(&m_clubAlpha);
                break;
            case Clubs_And_Bucklers:
                alpha->addMeleeWeapon(&m_clubAlpha);
                break;
        }
        addModel(alpha);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            switch (m_weaponOption) {
                case Javelins_Daggers_And_Bucklers:
                    model->addMissileWeapon(&m_javelin);
                    model->addMeleeWeapon(&m_dagger);
                    break;
                case Boltspitters_Daggers_And_Bucklers:
                    model->addMissileWeapon(&m_boltspitter);
                    model->addMeleeWeapon(&m_dagger);
                    break;
                case Boltspitters_And_Clubs:
                    model->addMissileWeapon(&m_boltspitter);
                    model->addMeleeWeapon(&m_club);
                    break;
                case Clubs_And_Bucklers:
                    model->addMeleeWeapon(&m_club);
                    break;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Skinks::Create(const ParameterList &parameters) {
        auto unit = new Skinks();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Boltspitters_Daggers_And_Bucklers);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Skinks::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {Javelins_Daggers_And_Bucklers, Boltspitters_And_Clubs,
                                                       Boltspitters_Daggers_And_Bucklers, Clubs_And_Bucklers};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Boltspitters_Daggers_And_Bucklers, weapons),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Skinks", factoryMethod);
        }
    }

    std::string Skinks::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Javelins_Daggers_And_Bucklers) { return "Javelins, Daggers and Bucklers"; }
            else if (parameter.intValue == Boltspitters_And_Clubs) { return "Boltspitters and Clubs"; }
            else if (parameter.intValue ==
                     Boltspitters_Daggers_And_Bucklers) { return "Boltspitters, Daggers and Bucklers"; }
            else if (parameter.intValue == Clubs_And_Bucklers) { return "Moonstone Clubs and Bucklers"; }
        }
        return SeraphonBase::ValueToString(parameter);
    }

    int Skinks::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Javelins, Daggers and Bucklers") { return Javelins_Daggers_And_Bucklers; }
        else if (enumString == "Boltspitters and Clubs") { return Boltspitters_And_Clubs; }
        else if (enumString == "Boltspitters, Daggers and Bucklers") { return Boltspitters_Daggers_And_Bucklers; }
        else if (enumString == "Moonstone Clubs and Bucklers") { return Clubs_And_Bucklers; }
        return SeraphonBase::EnumStringToInt(enumString);
    }

    int Skinks::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = SeraphonBase::toSaveModifier(weapon, attacker);

        if (m_weaponOption != Boltspitters_And_Clubs) {
            // Star-bucklers
            modifier++;
        }

        return modifier;
    }

    int Skinks::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Skinks::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = SeraphonBase::extraAttacks(attackingModel, weapon, target);

        // Swarming Cohort
        if (remainingModels() >= 15) extra++;

        return extra;
    }

} //namespace Seraphon
