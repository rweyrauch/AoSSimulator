/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Vargheists.h>
#include <UnitFactory.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 150;
    static const int POINTS_MAX_UNIT_SIZE = 600;

    bool Vargheists::s_registered = false;

    Vargheists::Vargheists() :
            LegionOfNagashBase("Vargheists", 12, WOUNDS, 10, 5, true),
            m_fangsAndTalons(Weapon::Type::Melee, "Murderous Fangs and Talons", 1, 3, 3, 3, -1, 2),
            m_fangsAndTalonsVargoyle(Weapon::Type::Melee, "Murderous Fangs and Talons", 1, 4, 3, 3, -1, 2) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, VARGHEISTS};
        m_weapons = {&m_fangsAndTalons, &m_fangsAndTalonsVargoyle};
    }

    bool Vargheists::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        auto vargoyle = new Model(BASESIZE, wounds());
        vargoyle->addMeleeWeapon(&m_fangsAndTalonsVargoyle);
        addModel(vargoyle);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_fangsAndTalons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Vargheists::Create(const ParameterList &parameters) {
        auto unit = new Vargheists();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Vargheists::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Vargheists", factoryMethod);
        }
    }

    int Vargheists::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace Death
