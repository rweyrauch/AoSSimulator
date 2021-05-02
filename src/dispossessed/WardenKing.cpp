/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/WardenKing.h>
#include <UnitFactory.h>
#include "DispossessedPrivate.h"

namespace Dispossessed {
    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool WardenKing::s_registered = false;

    WardenKing::WardenKing() :
            Dispossessed("Warden King", 4, g_wounds, 8, 4, false, g_pointsPerUnit),
            m_runeWeapon(Weapon::Type::Melee, "Rune Weapon", 1, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HERO, WARDEN_KING};
        m_weapons = {&m_runeWeapon};
    }

    void WardenKing::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_runeWeapon);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *WardenKing::Create(const ParameterList &parameters) {
        auto unit = new WardenKing();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void WardenKing::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WardenKing::Create,
                    Dispossessed::ValueToString,
                    Dispossessed::EnumStringToInt,
                    WardenKing::ComputePoints,
                    {
                            EnumParameter("Grudge", g_grudge[0], g_grudge),
                            BoolParameter("General")
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Warden King", factoryMethod);
        }
    }

    int WardenKing::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Dispossessed