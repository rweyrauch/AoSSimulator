/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/VampireLord.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 32;
    static const int BASESIZE_WITH_STEED = 60; // x35 oval
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    bool VampireLord::s_registered = false;

    Unit *VampireLord::Create(const ParameterList &parameters) {
        auto unit = new VampireLord();

        bool withSteed = GetBoolParam("Steed", parameters, false);
        bool withWings = GetBoolParam("Wings", parameters, false);
        bool chalice = GetBoolParam("Chalice of Blood", parameters, true);

        auto legion = (Legion)GetEnumParam("Legion", parameters, GrandHostOfNagash);
        unit->setLegion(legion);

        bool ok = unit->configure(withSteed, withWings, chalice);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int VampireLord::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void VampireLord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Boolean, "Steed", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Boolean, "Wings", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Boolean, "Chalice of Blood", SIM_FALSE, SIM_FALSE, SIM_FALSE, SIM_FALSE},
                            {ParamType::Enum, "Legion", Legion::GrandHostOfNagash, Legion ::GrandHostOfNagash, Legion::LegionOfBlood, 1},
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Vampire Lord", factoryMethod);
        }
    }

    VampireLord::VampireLord() :
            LegionOfNagashBase("Vampire Lord", 5, WOUNDS, 10, 4, false),
            m_blades(Weapon::Type::Melee, "Spirit-possessed Blades", 1, 4, 3, 3, -1, RAND_D3),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Nightmare's Hooves and Teeth", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, HERO, WIZARD, VAMPIRE_LORD};
        m_weapons = {&m_blades, &m_hoovesAndTeeth};
    }

    bool VampireLord::configure(bool withSteed, bool withWings, bool chalice) {
        auto model = new Model(withSteed ? BASESIZE_WITH_STEED : BASESIZE, wounds());

        if (withSteed) {
            model->addMeleeWeapon(&m_hoovesAndTeeth);
            m_move = 10;
        }
        else if (withWings) {
            m_move = 10;
            m_fly = true;
        }

        model->addMeleeWeapon(&m_blades);
        addModel(model);

        m_haveChaliceOfBlood = chalice;

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void VampireLord::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            deathlyInvocations(3, 12.0f);

            // Chalice of Blood
            if (m_haveChaliceOfBlood && !m_usedChaliceOfBlood && remainingWounds() < wounds()) {
                heal(Dice::rollD6());
                m_usedChaliceOfBlood = true;
            }
        }
    }

    void VampireLord::onRestore() {
        Unit::onRestore();

        m_usedChaliceOfBlood = false;
    }

    Wounds VampireLord::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(1);

        return Unit::onEndCombat(player);
    }

} // namespace Death
