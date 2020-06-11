/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <stormcast/TheFarstriders.h>

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 2;
    static const int POINTS_PER_UNIT = 100;

    bool TheFarstriders::s_registered = false;

    TheFarstriders::TheFarstriders() :
            StormcastEternal("The Farstriders", 6, WOUNDS, 7, 4, false),
            m_boltstormPistol(Weapon::Type::Missile, "Boltstorm Pistol", 9, 2, 3, 4, 0, 1),
            m_shockHandaxe(Weapon::Type::Melee, "Shock Handaxe", 1, 2, 4, 3, 0, 1),
            m_stormSaber(Weapon::Type::Melee, "Storm Sabre", 1, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, ANGELOS, VANGUARD_HUNTERS,
                      THE_FARSTRIDERS};
        m_weapons = {&m_boltstormPistol, &m_shockHandaxe, &m_stormSaber};

        // Tireless Hunters
        m_runAndShoot = true;
    }

    bool TheFarstriders::configure() {
        auto sanson = new Model(BASESIZE, wounds());
        sanson->setName("Sanson");
        sanson->addMissileWeapon(&m_boltstormPistol);
        sanson->addMeleeWeapon(&m_shockHandaxe);
        addModel(sanson);

        auto almeric = new Model(BASESIZE, wounds());
        almeric->setName("Almeric");
        almeric->addMissileWeapon(&m_boltstormPistol);
        almeric->addMeleeWeapon(&m_shockHandaxe);
        addModel(almeric);

        auto elias = new Model(BASESIZE, wounds());
        elias->setName("Elias");
        elias->addMissileWeapon(&m_boltstormPistol);
        elias->addMeleeWeapon(&m_stormSaber);
        addModel(elias);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *TheFarstriders::Create(const ParameterList &parameters) {
        auto unit = new TheFarstriders();
        unit->setStormhost(Stormhost::Hammers_of_Sigmar);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TheFarstriders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("The Farstriders", factoryMethod);
        }
    }

    int TheFarstriders::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals
