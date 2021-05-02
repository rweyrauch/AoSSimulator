/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "mawtribes/Hrothgorn.h"

namespace OgorMawtribes {
    static const int g_basesize = 50;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 120;

    bool Hrothgorn::s_registered = false;

    bool Hrothgorn::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *Hrothgorn::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto general = GetBoolParam("General", parameters, false);
            return new Hrothgorn(general);
        }
        return nullptr;
    }

    void Hrothgorn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Hrothgorn::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    Hrothgorn::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Hrothgorn", factoryMethod);
        }
    }

    Hrothgorn::Hrothgorn(bool isGeneral) :
            MawtribesBase(Mawtribe::Winterbite, "Hrothgorn", 6, g_wounds, 7, 5, false, g_pointsPerUnit) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, WINTERBITE, HERO, ICEBROW_HUNTER,
                      HROTHGORN};
        m_weapons = {&m_trapLauncher, &m_knife, &m_bite};
        m_battleFieldRole = Role::Leader;
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_trapLauncher);
        model->addMeleeWeapon(&m_knife);
        model->addMeleeWeapon(&m_bite);

        addModel(model);
    }

    int Hrothgorn::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
