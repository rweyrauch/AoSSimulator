/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/TheWurmspat.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 80;

    bool TheWurmspat::s_registered = false;

    Unit *TheWurmspat::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        return new TheWurmspat(legion);
    }

    void TheWurmspat::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheWurmspat::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    TheWurmspat::ComputePoints,
                    {
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("The Wurmspat", factoryMethod);
        }
    }

    TheWurmspat::TheWurmspat(PlagueLegion legion) :
            NurgleBase(legion, "The Wurmspat", 4, g_wounds, 8, 4, false, g_pointsPerUnit),
            m_blightedWeapons(Weapon::Type::Melee, "Blighted Weapon", 1, 3, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, BLESSED_SONS, PUTRID_BLIGHTKINGS, THE_WURMSPAT};
        m_weapons = {&m_blightedWeapons};

        auto sepsimus = new Model(g_basesize, wounds());
        sepsimus->addMeleeWeapon(&m_blightedWeapons);
        addModel(sepsimus);

        auto ghulgoch = new Model(g_basesize, wounds());
        ghulgoch->addMeleeWeapon(&m_blightedWeapons);
        addModel(ghulgoch);
    }

    int TheWurmspat::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Blighted Weapons
        if (unmodifiedHitRoll == 6) {
            // each 6 inflicts D6 hits
            return Dice::RollD6();
        }

        return NurgleBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int TheWurmspat::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
