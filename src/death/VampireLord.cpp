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

        bool ok = unit->configure(withSteed, withWings);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int VampireLord::ComputePoints(int numModels) {
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

    bool VampireLord::configure(bool withSteed, bool withWings) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        model->addMeleeWeapon(&m_blades);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }
} // namespace Death
