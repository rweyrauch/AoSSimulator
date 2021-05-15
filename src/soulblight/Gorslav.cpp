/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Gorslav.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 120;

    bool GorslavTheGravekeeper::s_registered = false;

    Unit *GorslavTheGravekeeper::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new GorslavTheGravekeeper(general);
    }

    int GorslavTheGravekeeper::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void GorslavTheGravekeeper::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Gorslav the Gravekeeper", factoryMethod);
        }
    }

    GorslavTheGravekeeper::GorslavTheGravekeeper(bool isGeneral) :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Gorslav the Gravekeeper", 4, g_wounds, 10, 6, false, g_pointsPerUnit) {

        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, HERO, GORSLAV_THE_GRAVEKEEPER};
        m_weapons = {&m_spade};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_spade);
        model->setName("Gorslav the Gravekeeper");
        addModel(model);
    }

} // namespace Soulblight
