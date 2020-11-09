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
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 150;
    static const int g_pointsMaxUnitSize = 600;

    bool Vargheists::s_registered = false;

    Vargheists::Vargheists() :
            LegionOfNagashBase("Vargheists", 12, g_wounds, 10, 5, true),
            m_fangsAndTalons(Weapon::Type::Melee, "Murderous Fangs and Talons", 1, 3, 3, 3, -1, 2),
            m_fangsAndTalonsVargoyle(Weapon::Type::Melee, "Murderous Fangs and Talons", 1, 4, 3, 3, -1, 2) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, VARGHEISTS};
        m_weapons = {&m_fangsAndTalons, &m_fangsAndTalonsVargoyle};
    }

    bool Vargheists::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        auto vargoyle = new Model(g_basesize, wounds());
        vargoyle->addMeleeWeapon(&m_fangsAndTalonsVargoyle);
        addModel(vargoyle);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_fangsAndTalons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Vargheists::Create(const ParameterList &parameters) {
        auto unit = new Vargheists();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Vargheists", factoryMethod);
        }
    }

    int Vargheists::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Death
