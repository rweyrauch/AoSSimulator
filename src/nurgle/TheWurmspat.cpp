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
        auto unit = new TheWurmspat();

        auto legion = (PlagueLegion)GetEnumParam("Plague Legion", parameters, (int)PlagueLegion::None);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    TheWurmspat::TheWurmspat() :
            NurgleBase("The Wurmspat", 4, g_wounds, 8, 4, false),
            m_blightedWeapons(Weapon::Type::Melee, "Blighted Weapon", 1, 3, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, BLESSED_SONS, PUTRID_BLIGHTKINGS, THE_WURMSPAT};
        m_weapons = {&m_blightedWeapons};
    }

    bool TheWurmspat::configure() {
        auto sepsimus = new Model(g_basesize, wounds());
        sepsimus->addMeleeWeapon(&m_blightedWeapons);
        addModel(sepsimus);

        auto ghulgoch = new Model(g_basesize, wounds());
        ghulgoch->addMeleeWeapon(&m_blightedWeapons);
        addModel(ghulgoch);

        m_points = g_pointsPerUnit;

        return true;
    }

    int TheWurmspat::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Blighted Weapons
        if (unmodifiedHitRoll == 6) {
            // each 6 inflicts D6 hits
            return Dice::RollD6();
        }

        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int TheWurmspat::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
