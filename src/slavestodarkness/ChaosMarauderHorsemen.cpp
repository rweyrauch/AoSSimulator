/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosMarauderHorsemen.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 65;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 110;
    static const int g_pointsMaxUnitSize = 330;

    bool ChaosMarauderHorsemen::s_registered = false;

    Unit *ChaosMarauderHorsemen::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Axe_And_Shield);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);

        return new ChaosMarauderHorsemen(legion, mark, numModels, weapons, iconBearer, hornblower);
    }

    void ChaosMarauderHorsemen::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Axe_And_Shield, Javelin_And_Shield, Flail};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Axe_And_Shield, weapons),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };

            s_registered = UnitFactory::Register("Chaos Marauders", factoryMethod);
        }
    }

    ChaosMarauderHorsemen::ChaosMarauderHorsemen(DamnedLegion legion, MarkOfChaos mark,  int numModels, WeaponOption weapons, bool iconBearer, bool hornblower) :
            SlavesToDarknessBase("Chaos Marauder Horsemen", 12, g_wounds, 5, 6, false) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_MARAUDER_HORSEMEN};
        m_weapons = {&m_javelinMissile, &m_axe, &m_flail, &m_javelin, &m_axeMaster, &m_flailMaster, &m_javelinMaster,
                     &m_hooves};
        m_hasMount = true;
        m_hooves.setMount(true);

        s_globalBraveryMod.connect(this, &ChaosMarauderHorsemen::iconBearer, &m_connection);

        setDamnedLegion(legion);
        setMarkOfChaos(mark);

        // Feigned Flight
        m_retreatAndCharge = true;
        m_retreatAndShoot = true;

        m_weaponOption = weapons;

        auto leader = new Model(g_basesize, wounds());
        if (weapons == Axe_And_Shield) {
            leader->addMeleeWeapon(&m_axeMaster);
        } else if (weapons == Flail) {
            leader->addMeleeWeapon(&m_flailMaster);
        } else if (weapons == Javelin_And_Shield) {
            leader->addMeleeWeapon(&m_javelinMaster);
            leader->addMissileWeapon(&m_javelinMissile);
        }
        leader->addMeleeWeapon(&m_hooves);
        leader->setName("Marauder Chieftain");
        addModel(leader);

        if (iconBearer) {
            auto model = new Model(g_basesize, wounds());
            model->setName(Model::IconBearer);
            if (weapons == Axe_And_Shield)
                model->addMeleeWeapon(&m_axe);
            else if (weapons == Flail)
                model->addMeleeWeapon(&m_flail);
            else if (weapons == Javelin_And_Shield) {
                model->addMissileWeapon(&m_javelinMissile);
                model->addMeleeWeapon(&m_javelin);
            }
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        if (hornblower) {
            auto model = new Model(g_basesize, wounds());
            model->setName(Model::Hornblower);
            if (weapons == Axe_And_Shield)
                model->addMeleeWeapon(&m_axe);
            else if (weapons == Flail)
                model->addMeleeWeapon(&m_flail);
            else if (weapons == Javelin_And_Shield) {
                model->addMissileWeapon(&m_javelinMissile);
                model->addMeleeWeapon(&m_javelin);
            }
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        for (auto i = (int) m_models.size(); i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Axe_And_Shield)
                model->addMeleeWeapon(&m_axe);
            else if (weapons == Flail)
                model->addMeleeWeapon(&m_flail);
            else if (weapons == Javelin_And_Shield) {
                model->addMissileWeapon(&m_javelinMissile);
                model->addMeleeWeapon(&m_javelin);
            }
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        // Darkwood Shields
        if (m_weaponOption == Axe_And_Shield || m_weaponOption == Javelin_And_Shield) {
            m_save = 5;
        }

        m_points = ComputePoints(numModels);
    }

    ChaosMarauderHorsemen::~ChaosMarauderHorsemen() {
        m_connection.disconnect();
    }

    std::string ChaosMarauderHorsemen::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Axe_And_Shield) {
                return "Barbarian Axe and Darkwood Shield";
            } else if (parameter.intValue == Flail) {
                return "Barbarian Flail";
            } else if (parameter.intValue == Javelin_And_Shield) {
                return "Marauder Javelin and Darkwood Shield";
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int ChaosMarauderHorsemen::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Barbarian Axe and Darkwood Shield") {
            return Axe_And_Shield;
        } else if (enumString == "Barbarian Flail") {
            return Flail;
        } else if (enumString == "Marauder Javelin and Darkwood Shield") {
            return Javelin_And_Shield;
        }
        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int ChaosMarauderHorsemen::runModifier() const {
        auto modifier = SlavesToDarknessBase::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) modifier += 1;
        return modifier;
    }

    int ChaosMarauderHorsemen::chargeModifier() const {
        auto modifier = SlavesToDarknessBase::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) modifier += 1;
        return modifier;
    }

    int ChaosMarauderHorsemen::toHitModifier(const Weapon *weapon, const Unit *target) const {
        // Barbarian Hordes
        auto modifier = SlavesToDarknessBase::toHitModifier(weapon, target);
        if (remainingModels() >= 10) modifier++;
        return modifier;
    }

    int ChaosMarauderHorsemen::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Barbarian Hordes
        auto rend = SlavesToDarknessBase::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
        if (remainingModels() >= 10) rend--;
        return rend;
    }

    int ChaosMarauderHorsemen::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int ChaosMarauderHorsemen::iconBearer(const Unit *unit) {
        // Icon Bearer
        if (isNamedModelAlive(Model::IconBearer) && (unit->owningPlayer() != owningPlayer()) &&
            (distanceTo(unit) <= 6.0)) {
            return -1;
        }
        return 0;
    }

} //SlavesToDarkness