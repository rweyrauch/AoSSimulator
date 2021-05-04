/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GiantRats.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 200;

    bool GiantRats::s_registered = false;

    Unit *GiantRats::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new GiantRats(numModels, ComputePoints(parameters));
    }

    int GiantRats::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void GiantRats::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Giant Rats", factoryMethod);
        }
    }

    GiantRats::GiantRats(int numModels, int points) :
            Skaventide("Giant Rats", 8, g_wounds, 3, NoSave, false, points),
            m_teeth(Weapon::Type::Melee, "Vicious Teeth", 1, 1, 4, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVENTIDE, CLANS_MOULDER, PACK, GIANT_RATS};
        m_weapons = {&m_teeth};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_teeth);
            addModel(model);
        }

        setTeethRange();
    }

    void GiantRats::setTeethRange() {
        if (remainingModels() >= 20) {
            m_teeth.setRange(3);
        } else if (remainingModels() >= 10) {
            m_teeth.setRange(2);
        } else {
            m_teeth.setRange(1);
        }
    }

} //namespace Skaven
