/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/UntamedBeasts.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 9;
    static const int MAX_UNIT_SIZE = 36;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool UntamedBeasts::s_registered = false;

    Unit *UntamedBeasts::Create(const ParameterList &parameters) {
        auto unit = new UntamedBeasts();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void UntamedBeasts::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    UntamedBeasts::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    UntamedBeasts::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };

            s_registered = UnitFactory::Register("Untamed Beasts", factoryMethod);
        }
    }

    UntamedBeasts::UntamedBeasts() :
            SlavesToDarknessBase("Untamed Beasts", 6, WOUNDS, 5, 6, false),
            m_harpoonFirstFang(Weapon::Type::Missile, "Jagged Harpoon", 8, 1, 4, 3, -1, 2),
            m_huntingWeapons(Weapon::Type::Melee, "Hunting Weapons", 1, 1, 4, 4, 0, 1),
            m_huntingWeaponsHeartEater(Weapon::Type::Melee, "Hunting Weapons (Heart-eater)", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, UNTAMED_BEASTS};
        m_weapons = {&m_harpoonFirstFang, &m_huntingWeapons, &m_huntingWeaponsHeartEater};

        // Unleash the Beast
        m_runAndCharge = true;
    }

    bool UntamedBeasts::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto hearteater = new Model(BASESIZE, wounds());
        hearteater->addMeleeWeapon(&m_huntingWeaponsHeartEater);
        hearteater->setName("Heart-eater");
        addModel(hearteater);

        auto firstfang = new Model(BASESIZE, wounds());
        firstfang->addMissileWeapon(&m_harpoonFirstFang);
        firstfang->addMeleeWeapon(&m_huntingWeapons);
        firstfang->setName("First Fang");
        addModel(firstfang);

        auto prowler = new Model(BASESIZE, 2);
        prowler->addMeleeWeapon(&m_huntingWeapons);
        prowler->setName("Rocktusk Prowler");
        addModel(prowler);

        for (auto i = 3; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_huntingWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int UntamedBeasts::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //SlavesToDarkness