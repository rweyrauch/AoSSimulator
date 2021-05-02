/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ChaosKnights.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 75; // x42 oval
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 160;
    static const int g_pointsMaxUnitSize = 4 * g_pointsPerBlock;

    bool ChaosKnights::s_registered = false;

    Unit *ChaosKnights::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Ensorcelled_Weapon);
        auto doomWeapon = (WeaponOption) GetEnumParam("Doom Knight Weapon", parameters, Ensorcelled_Weapon);
        bool standardBearer = GetBoolParam("Standard Bearer", parameters, false);
        bool hornblower = GetBoolParam("Hornblower", parameters, false);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);

        return new ChaosKnights(legion, mark, numModels, weapons, doomWeapon, standardBearer, hornblower, ComputePoints(parameters));
    }

    void ChaosKnights::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Ensorcelled_Weapon, Cursed_Lance};
            static const std::array<int, 3> knightWeapons = {Ensorcelled_Weapon, Cursed_Lance, Cursed_Flail};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Ensorcelled_Weapon, weapons),
                            EnumParameter("Doom Knight Weapon", Ensorcelled_Weapon, knightWeapons),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };

            s_registered = UnitFactory::Register("Chaos Knights", factoryMethod);
        }
    }

    ChaosKnights::ChaosKnights(DamnedLegion legion, MarkOfChaos mark, int numModels, WeaponOption weapons, WeaponOption doomKnightWeapon,
                               bool standardBearer, bool hornblower, int points) :
            SlavesToDarknessBase("Chaos Knights", 10, g_wounds, 7, 4, false, points) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_KNIGHTS};
        m_weapons = {&m_ensorcelledWeapon, &m_lance, &m_ensorcelledWeaponLeader, &m_lanceLeader, &m_flailLeader, &m_hooves};
        m_hasMount = true;
        m_hooves.setMount(true);
        s_globalBraveryMod.connect(this, &ChaosKnights::terrifyingChampions, &m_terrifyingSlot);

        setDamnedLegion(legion);
        setMarkOfChaos(mark);

        auto leader = new Model(g_basesize, wounds());
        if (doomKnightWeapon == Ensorcelled_Weapon) {
            leader->addMeleeWeapon(&m_ensorcelledWeaponLeader);
        } else if (doomKnightWeapon == Cursed_Lance) {
            leader->addMeleeWeapon(&m_lanceLeader);
        } else if (doomKnightWeapon == Cursed_Flail) {
            leader->addMeleeWeapon(&m_flailLeader);
        }
        leader->addMeleeWeapon(&m_hooves);
        leader->setName("Doom Knight");
        addModel(leader);

        if (standardBearer) {
            auto model = new Model(g_basesize, wounds());
            model->setName(Model::StandardBearer);
            if (weapons == Ensorcelled_Weapon)
                model->addMeleeWeapon(&m_ensorcelledWeapon);
            else if (weapons == Cursed_Lance)
                model->addMeleeWeapon(&m_lance);
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        if (hornblower) {
            auto model = new Model(g_basesize, wounds());
            model->setName(Model::Hornblower);
            if (weapons == Ensorcelled_Weapon)
                model->addMeleeWeapon(&m_ensorcelledWeapon);
            else if (weapons == Cursed_Lance)
                model->addMeleeWeapon(&m_lance);
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        for (auto i = (int) m_models.size(); i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Ensorcelled_Weapon)
                model->addMeleeWeapon(&m_ensorcelledWeapon);
            else if (weapons == Cursed_Lance)
                model->addMeleeWeapon(&m_lance);
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }
    }

    ChaosKnights::~ChaosKnights() {
        m_terrifyingSlot.disconnect();
    }

    std::string ChaosKnights::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons" || std::string(parameter.name) == "Doom Knight Weapon") {
            if (parameter.intValue == Ensorcelled_Weapon) {
                return "Ensorcelled Weapon";
            } else if (parameter.intValue == Cursed_Lance) {
                return "Cursed Lance";
            } else if (parameter.intValue == Cursed_Flail) {
                return "Cursed Flail";
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int ChaosKnights::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ensorcelled Weapon") {
            return Ensorcelled_Weapon;
        } else if (enumString == "Cursed Lance") {
            return Cursed_Lance;
        } else if (enumString == "Cursed Flail") {
            return Cursed_Flail;
        }
        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    Wounds ChaosKnights::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Chaos Runeshield
        Dice::RollResult mortalSaves;
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int ChaosKnights::runModifier() const {
        auto modifier = SlavesToDarknessBase::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) modifier += 1;
        return modifier;
    }

    int ChaosKnights::chargeModifier() const {
        auto modifier = SlavesToDarknessBase::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) modifier += 1;
        return modifier;
    }

    int ChaosKnights::braveryModifier() const {
        auto modifier = SlavesToDarknessBase::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) modifier += 1;
        return modifier;
    }

    Wounds ChaosKnights::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Impaling Charge
        if (m_charged && (weapon->name() == m_lance.name())) {
            return {weapon->damage() + 1, 0};
        }
        return SlavesToDarknessBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int ChaosKnights::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Impaling Charge
        if (m_charged && (weapon->name() == m_lance.name())) {
            return weapon->rend() - 2;
        }
        return SlavesToDarknessBase::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int ChaosKnights::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int ChaosKnights::terrifyingChampions(const Unit *unit) {
        // Terrifying Champions
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 3.0)) {
            return -1;
        }
        return 0;
    }

} //SlavesToDarkness