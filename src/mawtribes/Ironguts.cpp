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
    static const int BASESIZE = 40;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 4;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 220;
    static const int POINTS_MAX_UNIT_SIZE = 660;


    bool Ironguts::s_registered = false;

    Unit *Ironguts::Create(const ParameterList &parameters) {
        auto unit = new Ironguts();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool mawBearer = GetBoolParam("Rune Maw Bearer", parameters, true);
        bool bellower = GetBoolParam("Bellower", parameters, true);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        bool ok = unit->configure(numModels, mawBearer, bellower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Ironguts::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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

    Ironguts::Ironguts() :
            MawtribesBase("Ironguts", 6, WOUNDS, 7, 4, false),
            m_bashingWeapon(Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 3, 3, 3, 0, 2),
            m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1),
            m_bashingWeaponGutlord(Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 4, 3, 3, 0, 2) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, IRONGUTS};
        m_weapons = {&m_bashingWeapon, &m_bite, &m_bashingWeaponGutlord};

        s_globalBraveryMod.connect(this, &Ironguts::bellower, &m_connection);
    }

    Ironguts::~Ironguts() {
        m_connection.disconnect();
    }

    bool Ironguts::configure(int numModels, bool runeMawBearer, bool bellower) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_runeMawBearer = runeMawBearer;
        m_bellower = bellower;

        auto gutlord = new Model(BASESIZE, wounds());
        gutlord->addMeleeWeapon(&m_bashingWeaponGutlord);
        gutlord->addMeleeWeapon(&m_bite);
        addModel(gutlord);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_bashingWeapon);
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Ironguts::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int Ironguts::bellower(const Unit *target) {
        // Bellower
        if (m_bellower && (target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 6.0)) {
            return -1;
        }
        return 0;
    }

} // namespace OgorMawtribes
