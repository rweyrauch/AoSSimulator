/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Belakor.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 50;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 240;

    bool Belakor::s_registered = false;

    Unit *Belakor::Create(const ParameterList &parameters) {
        auto unit = new Belakor();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, Undivided);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int Belakor::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void Belakor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Belakor::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    Belakor::ComputePoints,
                    {
                            {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers,
                             SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                            {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided,
                             SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Be'lakor", factoryMethod);
        }
    }

    Belakor::Belakor() :
        SlavesToDarknessBase("Be'lakor", 12, WOUNDS, 10, 4, true),
        m_blade(Weapon::Type::Melee, "Blade of Shadows", 1, 6, 3, 3, -1, 2) {
        m_keywords = {CHAOS, DAEMON, SLAVES_TO_DARKNESS, UNDIVIDED, HERO, WIZARD, DAEMON_PRINCE, BELAKOR};
        m_weapons = {&m_blade};
    }

    bool Belakor::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace SlavesToDarkness


