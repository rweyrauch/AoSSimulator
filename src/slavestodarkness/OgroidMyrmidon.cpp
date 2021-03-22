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
        auto unit = new OgroidMyrmidon();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    OgroidMyrmidon::OgroidMyrmidon() :
            SlavesToDarknessBase("Ogroid Myrmidon", 6, g_wounds, 8, 4, false),
            m_spearMissile(Weapon::Type::Missile, "Gladiator Spear", 18, 1, 3, 3, -1, RAND_D3),
            m_spear(Weapon::Type::Melee, "Gladiator Spear", 2, 6, 3, 3, -1, 1),
            m_horns(Weapon::Type::Melee, "Great Horns", 1, 1, 3, 3, -2, 3) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, HERO, EYE_OF_THE_GODS, OGROID_MYRMIDON};
        m_weapons = {&m_spearMissile, &m_spear, &m_horns};
        m_battleFieldRole = Role::Leader;
    }

    bool OgroidMyrmidon::configure() {
        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_spearMissile);
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_horns);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int OgroidMyrmidon::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Arcane Fury
        if ((unmodifiedHitRoll == 6) && (!weapon->isMissile())) {
            return 2;
        }
        return SlavesToDarknessBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int OgroidMyrmidon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

}//namespace SlavesToDarkness

