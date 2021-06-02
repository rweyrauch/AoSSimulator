/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/WatchCaptainHalgrim.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool WatchCaptainHalgrim::s_registered = false;

    Unit *WatchCaptainHalgrim::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new WatchCaptainHalgrim(general);
    }

    int WatchCaptainHalgrim::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void WatchCaptainHalgrim::Init() {
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
            s_registered = UnitFactory::Register("Watch Captain Halgrim", factoryMethod);
        }
    }

    WatchCaptainHalgrim::WatchCaptainHalgrim(bool isGeneral) :
        SoulblightBase(CursedBloodline::Vyrkos_Dynasty, "Watch Captain Halgrim", 4, g_wounds, 10, 4, false, g_pointsPerUnit) {

        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, VYRKOS_DYNASTY, HERO, DEATHRATTLE, WATCH_CAPTAIN_HALGRIM};
        m_weapons = {&m_halberd};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_halberd);
        model->setName("Watch Captain Halgrim");
        addModel(model);
    }

    Wounds WatchCaptainHalgrim::weaponDamage(const Model *attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                             int woundRoll) const {
        // Cursed Halberd
        if (hitRoll == 6) {
            return {weapon->damage(), 1, Wounds::Source::Weapon_Melee, weapon};
        }
        return SoulblightBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} // namespace Soulblight
