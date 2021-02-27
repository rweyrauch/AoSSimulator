/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/BloodKnights.h>
#include <UnitFactory.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 15;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 3;

    bool BloodKnights::s_registered = false;

    BloodKnights::BloodKnights() :
            LegionOfNagashBase("Blood Knights", 10, g_wounds, 10, 4, false),
            m_templarLanceOrBlade(Weapon::Type::Melee, "Templar Lance or Blade", 1, 3, 3, 3, -1, 1),
            m_templarLanceOrBladeKastellan(Weapon::Type::Melee, "Template Lance or Blade", 1, 4, 3, 3, -1, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Nightmare's Hooves and Teeth", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, BLOOD_KNIGHTS};
        m_weapons = {&m_templarLanceOrBlade, &m_templarLanceOrBladeKastellan, &m_hoovesAndTeeth};
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);

        s_globalBraveryMod.connect(this, &BloodKnights::standardBearerBraveryMod, &m_standardSlot);
    }

    BloodKnights::~BloodKnights() {
        m_standardSlot.disconnect();
    }

    bool BloodKnights::configure(int numModels, bool standardBearers, bool hornblowers) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

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
            }
            else if (hornblowers) {
                model->setName(Model::Hornblower);
                hornblowers = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BloodKnights::Create(const ParameterList &parameters) {
        auto unit = new BloodKnights();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, standardBearers, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BloodKnights::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearers"),
                            BoolParameter("Hornblowers"),
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Blood Knights", factoryMethod);
        }
    }

    Wounds BloodKnights::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Martial Fury
        if (m_charged && weapon->name() == m_templarLanceOrBlade.name()) {
            return {weapon->damage() + Dice::RollD3(), 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BloodKnights::toSaveModifier(const Weapon *weapon, const Unit* attacker) const {
        int modifier = Unit::toSaveModifier(weapon, attacker);

        // Bloodshields
        if (weapon->rend() == 0) {
            modifier += 1;
        }

        return modifier;
    }

    int BloodKnights::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int BloodKnights::rollChargeDistance() const {
        // Hornblower
        auto dist = Unit::rollChargeDistance();
        return std::max(6, dist);
    }

    int BloodKnights::standardBearerBraveryMod(const Unit *unit) {
        if (isNamedModelAlive(Model::StandardBearer) && !isFriendly(unit) && (distanceTo(unit) <= 6.0)) return -1;
        return 0;
    }

    void BloodKnights::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(1);

        Unit::onEndCombat(player);
    }

} //namespace Death
