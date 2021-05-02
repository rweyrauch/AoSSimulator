/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "sylvaneth/SkaethsWildHunt.h"

namespace Sylvaneth {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 120;

    bool SkaethsWildHunt::s_registered = false;

    Unit *SkaethsWildHunt::Create(const ParameterList &parameters) {
        return new SkaethsWildHunt();
    }

    void SkaethsWildHunt::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SkaethsWildHunt::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    SkaethsWildHunt::ComputePoints,
                    {
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Skaeth's Wild Hunt", factoryMethod);
        }
    }

    SkaethsWildHunt::SkaethsWildHunt() :
            SylvanethBase("Skaeth's Wild Hunt", 6, g_wounds, 6, 5, false, g_pointsPerUnit) {
        m_keywords = {ORDER, SYLVANETH, OAKENBROW, SKAETHS_WILD_HUNT};
        m_weapons = {&m_seekerBow, &m_javalin, &m_javalinMelee, &m_huntingWeapon, &m_teethAndClaws};

        setGlade(Glade::Oakenbrow);

        // Fleet of Foot
        m_runAndCharge = true;
        m_runAndShoot = true;

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        auto skaeth = new Model(g_basesize, wounds());
        skaeth->addMissileWeapon(&m_javalin);
        skaeth->addMeleeWeapon(&m_javalinMelee);
        skaeth->setName("Skaeth");
        addModel(skaeth);

        auto althaen = new Model(g_basesize, wounds());
        althaen->addMissileWeapon(&m_seekerBow);
        althaen->addMeleeWeapon(&m_huntingWeapon);
        althaen->setName("Althaen");
        addModel(althaen);

        auto karthaen = new Model(g_basesize, wounds());
        karthaen->addMeleeWeapon(&m_huntingWeapon);
        karthaen->setName("Karthaen");
        addModel(karthaen);

        auto sheoch = new Model(g_basesize, wounds());
        sheoch->addMeleeWeapon(&m_huntingWeapon);
        sheoch->setName("Sheoch");
        addModel(sheoch);

        auto lighaen = new Model(g_basesize, wounds());
        lighaen->addMeleeWeapon(&m_teethAndClaws);
        lighaen->setName("Lighaen");
        addModel(lighaen);

        m_points = g_pointsPerUnit;
    }

    int SkaethsWildHunt::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Sylvaneth