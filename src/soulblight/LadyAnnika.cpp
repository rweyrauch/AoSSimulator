/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/LadyAnnika.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 30;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 0;

    bool LadyAnnika::s_registered = false;

    Unit *LadyAnnika::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new LadyAnnika(general);
    }

    int LadyAnnika::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void LadyAnnika::Init() {
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
            s_registered = UnitFactory::Register("Lady Annika", factoryMethod);
        }
    }

    LadyAnnika::LadyAnnika(bool isGeneral) :
            SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Lady Annika The Thirsting Blade", 6, g_wounds, 10, 4, false, g_pointsPerUnit) {

        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, HERO, LADY_ANNIKA};
        m_weapons = {&m_blade};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->setName("Lady Annika");
        addModel(model);
    }

    Wounds LadyAnnika::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Supernatural Speed
        int numSixes = 0;
        return ignoreWounds(wounds, 4, numSixes);
    }

    void LadyAnnika::onEndCombat(PlayerId player) {
        SoulblightBase::onEndCombat(player);

        // Kiss of the Blade Proboscian
        if (m_currentRecord.m_enemyModelsSlain > 0) {
            heal(wounds());
        }
    }

} // namespace Soulblight
