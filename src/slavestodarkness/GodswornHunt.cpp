/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/GodswornHunt.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 60;

    bool GodswornHunt::s_registered = false;

    Unit *GodswornHunt::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new GodswornHunt(legion);
    }

    int GodswornHunt::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void GodswornHunt::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GodswornHunt::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    GodswornHunt::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Godsworn Hunt", factoryMethod);
        }
    }

    GodswornHunt::GodswornHunt(DamnedLegion legion) :
            SlavesToDarknessBase("Godsworn Hunt", 6, g_wounds, 6, 6, false) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, GODSWORN_HUNT};
        m_weapons = {&m_huntingBow, &m_javelin, &m_knife, &m_greatWeapon, &m_bowMelee, &m_bite};

        setDamnedLegion(legion);

        auto jagathra = new Model(g_basesize, wounds());
        jagathra->addMissileWeapon(&m_javelin);
        jagathra->addMeleeWeapon(&m_knife);
        jagathra->setName("Jagathra");
        addModel(jagathra);

        auto shond = new Model(g_basesize, wounds());
        shond->addMeleeWeapon(&m_greatWeapon);
        shond->setName("Shond Head-Claimer");
        addModel(shond);

        auto grundann = new Model(g_basesize, wounds());
        grundann->addMeleeWeapon(&m_greatWeapon);
        grundann->setName("Grundann Blood-Eye");
        addModel(grundann);

        auto ollo = new Model(g_basesize, wounds());
        ollo->addMissileWeapon(&m_huntingBow);
        ollo->addMeleeWeapon(&m_bowMelee);
        ollo->setName("Ollo");
        addModel(ollo);

        auto grawl = new Model(g_basesize, wounds());
        grawl->addMeleeWeapon(&m_bite);
        grawl->setName("Grawl");
        addModel(grawl);

        m_points = ComputePoints(1);
    }

    Rerolls GodswornHunt::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Pact of Soul and Iron
        return Rerolls::Failed;
    }

    Rerolls GodswornHunt::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Pact of Soul and Iron
        if (target->hasKeyword(STORMCAST_ETERNAL)) return Rerolls::Failed;
        return SlavesToDarknessBase::toWoundRerolls(weapon, target);
    }

} // namespace SlavesToDarkness


