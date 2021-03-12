/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/Festus.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 140;

    bool FestusTheLeechlord::s_registered = false;

    Unit *FestusTheLeechlord::Create(const ParameterList &parameters) {
        auto unit = new FestusTheLeechlord();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto legion = (PlagueLegion)GetEnumParam("Plague Legion", parameters, (int)PlagueLegion::None);
        unit->setLegion(legion);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_mortalRotbringerLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void FestusTheLeechlord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FestusTheLeechlord::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    FestusTheLeechlord::ComputePoints,
                    {
                            EnumParameter("Lore", g_mortalRotbringerLore[0], g_mortalRotbringerLore),
                            BoolParameter("General"),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),

                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Festus the Leechlord", factoryMethod);
        }
    }

    FestusTheLeechlord::FestusTheLeechlord() :
            NurgleBase("Festus the Leechlord", 4, g_wounds, 7, 5, false),
            m_staff(Weapon::Type::Melee, "Plague Staff", 1, 2, 4, 3, 0, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, WIZARD, FESTUS_THE_LEECHLORD};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;
    }

    bool FestusTheLeechlord::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int FestusTheLeechlord::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle
