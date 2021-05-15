/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/Kritza.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 30;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool KritzaTheRatPrince::s_registered = false;

    Unit *KritzaTheRatPrince::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new KritzaTheRatPrince(general);
    }

    int KritzaTheRatPrince::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void KritzaTheRatPrince::Init() {
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
            s_registered = UnitFactory::Register("Kritza", factoryMethod);
        }
    }

    KritzaTheRatPrince::KritzaTheRatPrince(bool isGeneral) :
            SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Kritza The Rat Prince", 6, g_wounds, 10, 4, false, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, HERO, KRITZA};
        m_weapons = {&m_blade};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->setName("Kritza");
        addModel(model);
    }

} // namespace Soulblight
