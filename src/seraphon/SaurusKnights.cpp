/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusKnights.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool SaurusKnights::s_registered = false;

    SaurusKnights::SaurusKnights(WayOfTheSeraphon way, Constellation constellation, int numModels, WeaponOption weapons, bool iconBearer, bool wardrum, int points) :
            SeraphonBase("Saurus Knights", 8, g_wounds, 8, 4, false, points) {
        m_keywords = {ORDER, SERAPHON, SAURUS, COLD_ONE, SAURUS_KNIGHTS};
        m_weapons = {&m_celestiteBlade, &m_celestiteBladeAlpha, &m_celestiteSpear, &m_celestiteSpearAlpha, &m_jaws,
                     &m_coldOneJaws};
        m_hasMount = true;
        m_coldOneJaws.setMount(true);
        s_globalBraveryMod.connect(this, &SaurusKnights::stardrakeIcon, &m_connection);

        setWayOfTheSeraphon(way, constellation);

        m_weaponOption = weapons;

        // Add the Alpha
        auto alpha = new Model(g_basesize, wounds());
        if (m_weaponOption == Celestite_Blade) {
            alpha->addMeleeWeapon(&m_celestiteBladeAlpha);
        } else if (m_weaponOption == Celestite_Warspear) {
            alpha->addMeleeWeapon(&m_celestiteSpearAlpha);
        }
        alpha->addMeleeWeapon(&m_jaws);
        alpha->addMeleeWeapon(&m_coldOneJaws);
        addModel(alpha);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (m_weaponOption == Celestite_Blade) {
                model->addMeleeWeapon(&m_celestiteBlade);
            } else if (m_weaponOption == Celestite_Warspear) {
                model->addMeleeWeapon(&m_celestiteSpear);
            }
            model->addMeleeWeapon(&m_jaws);
            model->addMeleeWeapon(&m_coldOneJaws);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            } else if (wardrum) {
                model->setName("Wardrum");
                wardrum = false;
            }
            addModel(model);
        }
    }

    SaurusKnights::~SaurusKnights() {
        m_connection.disconnect();
    }

    Unit *SaurusKnights::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Celestite_Blade);
        bool iconBearer = GetBoolParam("Stardrake Icon", parameters, false);
        bool wardrum = GetBoolParam("Wardrum", parameters, false);
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);

        return new SaurusKnights(way, constellation, numModels, weapons, iconBearer, wardrum, ComputePoints(parameters));
    }

    std::string SaurusKnights::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Celestite_Blade) { return "Celestite Blade"; }
            else if (parameter.intValue == Celestite_Warspear) { return "Celestite Warspear"; }
        }
        return SeraphonBase::ValueToString(parameter);
    }

    int SaurusKnights::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Celestite Blade") { return Celestite_Blade; }
        else if (enumString == "Celestite Warspear") { return Celestite_Warspear; }
        return SeraphonBase::EnumStringToInt(enumString);
    }

    void SaurusKnights::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Celestite_Blade, Celestite_Warspear};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Celestite_Blade, weapons),
                            BoolParameter("Stardrake Icon"),
                            BoolParameter("Wardrum"),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Saurus Knights", factoryMethod);
        }
    }

    Wounds SaurusKnights::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Celestite Warspear
        if ((hitRoll >= 6) && m_charged && (weapon->name() == m_celestiteSpear.name())) {
            return {weapon->damage(), 1};
        }
        return SeraphonBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int SaurusKnights::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls SaurusKnights::chargeRerolls() const {
        if (isNamedModelAlive("Wardrum")) return Rerolls::Failed;

        return SeraphonBase::chargeRerolls();
    }

    int SaurusKnights::stardrakeIcon(const Unit *target) {
        // Icon Bearer
        if (isNamedModelAlive(Model::IconBearer) && (target->owningPlayer() != owningPlayer()) &&
            (distanceTo(target) <= 6.0)) {
            return -1;
        }

        return 0;
    }

} //namespace Seraphon
