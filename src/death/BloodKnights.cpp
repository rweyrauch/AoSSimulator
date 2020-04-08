/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/BloodKnights.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 15;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    bool BloodKnights::s_registered = false;

    BloodKnights::BloodKnights() :
            LegionOfNagashBase("Blood Knights", 10, WOUNDS, 10, 4, false),
            m_templarLanceOrBlade(Weapon::Type::Melee, "Templar Lance or Blade", 1, 3, 3, 3, -1, 1),
            m_templarLanceOrBladeKastellan(Weapon::Type::Melee, "Template Lance or Blade", 1, 4, 3, 3, -1, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Nightmare's Hooves and Teeth", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, BLOOD_KNIGHTS};
        m_weapons = {&m_templarLanceOrBlade, &m_templarLanceOrBladeKastellan, &m_hoovesAndTeeth};

        s_globalBraveryMod.connect(this, &BloodKnights::standardBearerBraveryMod, &m_standardSlot);
    }

    BloodKnights::~BloodKnights() {
        m_standardSlot.disconnect();
    }

    bool BloodKnights::configure(int numModels, bool standardBearers, bool hornblowers) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_standardBearers = standardBearers;
        m_hornblowers = hornblowers;

        auto kastellan = new Model(BASESIZE, wounds());
        kastellan->addMeleeWeapon(&m_templarLanceOrBladeKastellan);
        kastellan->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(kastellan);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_templarLanceOrBlade);
            model->addMeleeWeapon(&m_hoovesAndTeeth);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BloodKnights::Create(const ParameterList &parameters) {
        auto unit = new BloodKnights();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto legion = (Legion) GetEnumParam("Legion", parameters, GrandHostOfNagash);
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
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Boolean, "Standard Bearers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Boolean, "Hornblowers", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Enum, "Legion", Legion::GrandHostOfNagash, Legion::GrandHostOfNagash,
                             Legion::LegionOfBlood, 1},
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
            return {weapon->damage() + Dice::rollD3(), 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int BloodKnights::toSaveModifier(const Weapon *weapon) const {
        int modifier = Unit::toSaveModifier(weapon);

        // Bloodshields
        if (weapon->rend() == 0) {
            modifier += 1;
        }

        return modifier;
    }

    int BloodKnights::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int BloodKnights::rollChargeDistance() const {
        // Hornblower
        auto dist = Unit::rollChargeDistance();
        return std::max(6, dist);
    }

    int BloodKnights::standardBearerBraveryMod(const Unit *unit) {
        if (m_standardBearers && !isFriendly(unit) && (distanceTo(unit) <= 6.0)) return -1;
        return 0;
    }

    Wounds BloodKnights::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(1);

        return Unit::onEndCombat(player);
    }

} //namespace Death
