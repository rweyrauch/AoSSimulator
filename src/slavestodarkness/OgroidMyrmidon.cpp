/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "slavestodarkness/OgroidMyrmidon.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 60;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 140;

    bool OgroidMyrmidon::s_registered = false;

    Unit *OgroidMyrmidon::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new OgroidMyrmidon(legion);
    }

    void OgroidMyrmidon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OgroidMyrmidon::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    OgroidMyrmidon::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Ogroid Myrmidon", factoryMethod);
        }
    }

    OgroidMyrmidon::OgroidMyrmidon(DamnedLegion legion) :
            SlavesToDarknessBase(legion, "Ogroid Myrmidon", 6, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, HERO, EYE_OF_THE_GODS, OGROID_MYRMIDON};
        m_weapons = {&m_spearMissile, &m_spear, &m_horns};
        m_battleFieldRole = Role::Leader;

        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_spearMissile);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_horns);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<BuffAbilityCommandAbility>(this, "Pit Marshall", 12, 12, GamePhase::Combat,
                                                                                 Ability::Ignore_Battleshock, 1,
                                                                                 Abilities::Target::Friendly,
                                                                                 std::vector<Keyword>(CULTISTS)));
    }

    int OgroidMyrmidon::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Arcane Fury
        if ((unmodifiedHitRoll == 6) && (!weapon->isMissile())) {
            return 2;
        }
        return SlavesToDarknessBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int OgroidMyrmidon::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

}//namespace SlavesToDarkness

