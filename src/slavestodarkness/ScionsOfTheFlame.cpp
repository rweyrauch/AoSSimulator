/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ScionsOfTheFlame.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 8;
    static const int MAX_UNIT_SIZE = 32;
    static const int POINTS_PER_BLOCK = 0;
    static const int POINTS_MAX_UNIT_SIZE = 0;

    bool ScionsOfTheFlame::s_registered = false;

    Unit *ScionsOfTheFlame::Create(const ParameterList &parameters) {
        auto unit = new ScionsOfTheFlame();
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

    void ScionsOfTheFlame::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };

            s_registered = UnitFactory::Register("Scions of the Flame", factoryMethod);
        }
    }

    ScionsOfTheFlame::ScionsOfTheFlame() :
            SlavesToDarknessBase("Scions of the Flame", 6, WOUNDS, 5, 6, false),
            m_pots(Weapon::Type::Missile, "Flameburst Pots", 8, 1, 4, 3, 0, 1),
            m_scionWeapons(Weapon::Type::Melee, "Scion Weapons", 1, 1, 4, 4, 0, 1),
            m_scionWeaponsLeaders(Weapon::Type::Melee, "Scion Weapons", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, SCIONS_OF_THE_FLAME};
        m_weapons = {&m_pots, &m_scionWeapons, &m_scionWeaponsLeaders};
    }

    bool ScionsOfTheFlame::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto lord = new Model(BASESIZE, wounds());
        lord->addMissileWeapon(&m_pots);
        lord->addMeleeWeapon(&m_scionWeaponsLeaders);
        lord->setName("Blazing Lord");
        addModel(lord);

        auto immolator = new Model(BASESIZE, wounds());
        immolator->addMissileWeapon(&m_pots);
        immolator->addMeleeWeapon(&m_scionWeaponsLeaders);
        immolator->setName("Immolator");
        addModel(immolator);

        auto champion = new Model(BASESIZE, wounds()+1);
        champion->addMissileWeapon(&m_pots);
        champion->addMeleeWeapon(&m_scionWeapons);
        champion->setName("Brazen Champion");
        addModel(champion);

        auto priest = new Model(BASESIZE, wounds());
        priest->addMissileWeapon(&m_pots);
        priest->addMeleeWeapon(&m_scionWeapons);
        priest->setName("Inferno Priest");
        addModel(priest);

        for (auto i = (int) m_models.size(); i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_pots);
            model->addMeleeWeapon(&m_scionWeapons);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int ScionsOfTheFlame::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //SlavesToDarkness