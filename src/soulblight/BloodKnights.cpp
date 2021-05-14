/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/BloodKnights.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 3;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 195;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;

    bool BloodKnights::s_registered = false;

    BloodKnights::BloodKnights(CursedBloodline legion, int numModels, bool standardBearers, bool hornblowers, int points) :
            SoulblightBase(legion, "Blood Knights", 10, g_wounds, 10, 3, false, points) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, BLOOD_KNIGHTS};
        m_weapons = {&m_templarLanceOrBlade, &m_templarLanceOrBladeKastellan, &m_hoovesAndTeeth};
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);

        auto kastellan = new Model(g_basesize, wounds());
        kastellan->addMeleeWeapon(&m_templarLanceOrBladeKastellan);
        kastellan->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(kastellan);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_templarLanceOrBlade);
            model->addMeleeWeapon(&m_hoovesAndTeeth);
            if (standardBearers) {
                model->setName(Model::StandardBearer);
                standardBearers = false;
            } else if (hornblowers) {
                model->setName(Model::Hornblower);
                hornblowers = false;
            }
            addModel(model);
        }
    }

    BloodKnights::~BloodKnights() {
        m_standardSlot.disconnect();
    }

    Unit *BloodKnights::Create(const ParameterList &parameters) {
        auto legion = (CursedBloodline) GetEnumParam("Legion", parameters, g_legions[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);
        return new BloodKnights(legion, numModels, standardBearers, hornblowers, ComputePoints(parameters));
    }

    void BloodKnights::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearers"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Blood Knights", factoryMethod);
        }
    }

    Wounds BloodKnights::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Martial Fury
        if (m_charged && weapon->name() == m_templarLanceOrBlade.name()) {
            return {weapon->damage()+1, 0};
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int BloodKnights::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void BloodKnights::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(Dice::RollD3());

        SoulblightBase::onEndCombat(player);
    }

} //namespace Soulblight
