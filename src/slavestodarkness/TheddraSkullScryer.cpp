/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/TheddraSkullScryer.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 0;

    bool TheddraSkullscryer::s_registered = false;

    Unit *TheddraSkullscryer::Create(const ParameterList &parameters) {
        auto unit = new TheddraSkullscryer();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int TheddraSkullscryer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void TheddraSkullscryer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheddraSkullscryer::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    TheddraSkullscryer::ComputePoints,
                    {
                            {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers,
                             SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Theddra Skull-scryer", factoryMethod);
        }
    }

    TheddraSkullscryer::TheddraSkullscryer() :
            SlavesToDarknessBase("Theddra Skull-scryer", 6, WOUNDS, 7, 5, false),
            m_wand(Weapon::Type::Melee, "Darkoath Wand", 1, 2, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, HERO, WIZARD, GOD_SPEAKER, THEDDRA_SKULL_SCRYER};
        m_weapons = {&m_wand};
    }

    bool TheddraSkullscryer::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_wand);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace SlavesToDarkness


