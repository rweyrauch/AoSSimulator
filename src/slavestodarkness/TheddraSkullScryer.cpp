/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/TheddraSkullScryer.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 70;

    bool TheddraSkullscryer::s_registered = false;

    Unit *TheddraSkullscryer::Create(const ParameterList &parameters) {
        auto unit = new TheddraSkullscryer();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
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
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
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

    bool TheddraSkullscryer::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_wand);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Rerolls TheddraSkullscryer::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Pact of Soul and Iron
        return RerollFailed;
    }

    Rerolls TheddraSkullscryer::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Pact of Soul and Iron
        if (target->hasKeyword(STORMCAST_ETERNAL)) return RerollFailed;
        return Unit::toWoundRerolls(weapon, target);
    }

} // namespace SlavesToDarkness


