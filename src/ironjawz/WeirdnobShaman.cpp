/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ironjawz/WeirdnobShaman.h"

namespace Ironjawz {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    bool OrrukWeirdnobShaman::s_registered = false;

    Unit *OrrukWeirdnobShaman::Create(const ParameterList &parameters) {
        auto unit = new OrrukWeirdnobShaman();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, Ironjawz::Ironsunz);
        unit->setWarclan(warclan);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int OrrukWeirdnobShaman::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void OrrukWeirdnobShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukWeirdnobShaman::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    OrrukWeirdnobShaman::ComputePoints,
                    {
                            {ParamType::Enum, "Warclan", Ironjawz::Ironsunz,
                             Ironjawz::Ironsunz, Ironjawz::DaChoppas, 1},
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Weirdnob Shaman", factoryMethod);
        }
    }

    OrrukWeirdnobShaman::OrrukWeirdnobShaman() :
            Ironjawz("Orruk Weirdnob Shaman", 4, WOUNDS, 6, 5, false),
            m_staff(Weapon::Type::Melee, "Waaagh! Staff", 1, 3, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, HERO, WIZARD, WEIRDNOB_SHAMAN};
        m_weapons = {&m_staff};
    }

    bool OrrukWeirdnobShaman::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace Ironjawz