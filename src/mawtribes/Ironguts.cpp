/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Ironguts.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 4;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 220;
    static const int g_pointsMaxUnitSize = 660;

    bool Ironguts::s_registered = false;

    bool Ironguts::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *Ironguts::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            int numModels = GetIntParam("Models", parameters, g_minUnitSize);
            bool mawBearer = GetBoolParam("Rune Maw Bearer", parameters, true);
            bool bellower = GetBoolParam("Bellower", parameters, true);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            return new Ironguts(tribe, numModels, mawBearer, bellower);
        }
        return nullptr;
    }

    void Ironguts::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Ironguts::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    Ironguts::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Rune Maw Bearer"),
                            BoolParameter("Bellower"),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };

            s_registered = UnitFactory::Register("Ironguts", factoryMethod);
        }
    }

    Ironguts::~Ironguts() {
        m_connection.disconnect();
    }

    Ironguts::Ironguts(Mawtribe tribe, int numModels, bool runeMawBearer, bool bellower) :
        MawtribesBase(tribe, "Ironguts", 6, g_wounds, 7, 4, false) {

        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, IRONGUTS};
        m_weapons = {&m_bashingWeapon, &m_bite, &m_bashingWeaponGutlord};

        s_globalBraveryMod.connect(this, &Ironguts::bellower, &m_connection);

        auto gutlord = new Model(g_basesize, wounds());
        gutlord->addMeleeWeapon(&m_bashingWeaponGutlord);
        gutlord->addMeleeWeapon(&m_bite);
        addModel(gutlord);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_bashingWeapon);
            model->addMeleeWeapon(&m_bite);
            if (runeMawBearer) {
                model->setName("Rune Maw Bearer");
                runeMawBearer = false;
            } else if (bellower) {
                model->setName("Bellower");
                bellower = false;
            }

            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    int Ironguts::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Ironguts::bellower(const Unit *target) {
        // Bellower
        if (isNamedModelAlive("Bellower") && (target->owningPlayer() != owningPlayer()) &&
            (distanceTo(target) <= 6.0)) {
            return -1;
        }
        return 0;
    }

} // namespace OgorMawtribes
