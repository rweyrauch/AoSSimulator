/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Slaughterbrute.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 50;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 0;

    bool Slaughterbrute::s_registered = false;

    Unit *Slaughterbrute::Create(const ParameterList &parameters) {
        auto unit = new Slaughterbrute();

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

    int Slaughterbrute::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void Slaughterbrute::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Slaughterbrute::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    Slaughterbrute::ComputePoints,
                    {
                            {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers,
                             SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                            {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided,
                             SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch, 1},
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE}
            };
            s_registered = UnitFactory::Register("Slaughterbrute", factoryMethod);
        }
    }

    Slaughterbrute::Slaughterbrute() :
            SlavesToDarknessBase("Slaughterbrute", 10, WOUNDS, 7, 4, false),
            m_claws(Weapon::Type::Melee, "Razor-tipped Claws", 2, 6, 4, 3, -1, RAND_D3),
            m_jaws(Weapon::Type::Melee, "Mighty Jaws", 1, 2, 4, 1, 0, 3),
            m_talons(Weapon::Type::Melee, "Slashing Talons", 1, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, MONSTER, SLAVES_TO_DARKNESS, SLAUGHTERBRUTE};
        m_weapons = {&m_claws, &m_jaws, &m_talons};
    }

    bool Slaughterbrute::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace SlavesToDarkness


