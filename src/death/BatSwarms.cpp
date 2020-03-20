/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/BatSwarms.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 8;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    bool BatSwarms::s_registered = false;

    Unit *BatSwarms::Create(const ParameterList &parameters) {
        auto unit = new BatSwarms();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int BatSwarms::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    void BatSwarms::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Bat Swarms", factoryMethod);
        }
    }

    BatSwarms::BatSwarms() :
            LegionOfNagashBase("Bat Swarms", 12, WOUNDS, 10, NoSave, true),
            m_teeth(Weapon::Type::Melee, "Razor-sharp Teeth", 3, 5, 5, 5, 0, 1) {
        m_keywords = {DEATH, SOULBLIGHT, SUMMONABLE, BAT_SWARMS};
        m_weapons = {&m_teeth};
    }

    bool BatSwarms::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_teeth);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

} // namespace Death
