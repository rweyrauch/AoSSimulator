/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Vargheists.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 9;
    static const int g_pointsPerBlock = 155;
    static const int g_pointsMaxUnitSize = 3 * g_pointsPerBlock;

    bool Vargheists::s_registered = false;

    Vargheists::Vargheists(Legion legion, int numModels, int points) :
            SoulblightBase(legion, "Vargheists", 12, g_wounds, 10, 5, true, points),
            m_fangsAndTalons(Weapon::Type::Melee, "Murderous Fangs and Talons", 1, 3, 3, 3, -1, 2),
            m_fangsAndTalonsVargoyle(Weapon::Type::Melee, "Murderous Fangs and Talons", 1, 4, 3, 3, -1, 2) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, VARGHEISTS};
        m_weapons = {&m_fangsAndTalons, &m_fangsAndTalonsVargoyle};

        auto vargoyle = new Model(g_basesize, wounds());
        vargoyle->addMeleeWeapon(&m_fangsAndTalonsVargoyle);
        addModel(vargoyle);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_fangsAndTalons);
            addModel(model);
        }
    }

    Unit *Vargheists::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new Vargheists(legion, numModels, ComputePoints(parameters));
    }

    void Vargheists::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
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

    int Vargheists::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Soulblight
