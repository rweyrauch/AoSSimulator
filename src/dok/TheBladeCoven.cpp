/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dok/TheBladeCoven.h>
#include <UnitFactory.h>
#include <iostream>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int POINTS_PER_UNIT = 0;

    bool TheBladeCoven::s_registered = false;

    TheBladeCoven::TheBladeCoven() :
            DaughterOfKhaine("The Blade Coven", 8, WOUNDS, 8, 5, false),
            m_heartseekerBow(Weapon::Type::Missile, "Heartseeker Bow", 24, 1, 3, 3, -1, 1),
            m_sacrificialWeapons(Weapon::Type::Melee, "Sacrificial Weapons", 1, 3, 3, 4, 0, 1) {
        m_keywords = {ORDER, DAUGHTERS_OF_KHAINE, MELUSAI, THE_BLADE_COVEN};
        m_weapons = {&m_heartseekerBow, &m_sacrificialWeapons};
    }

    bool TheBladeCoven::configure() {

        auto kyrae = new Model(BASESIZE, wounds()+1);
        kyrae->addMissileWeapon(&m_heartseekerBow);
        kyrae->addMeleeWeapon(&m_sacrificialWeapons);
        kyrae->setName("Kyrae");
        addModel(kyrae);

        auto khamyss = new Model(BASESIZE, wounds());
        khamyss->addMeleeWeapon(&m_sacrificialWeapons);
        khamyss->setName("Khamyss");
        addModel(khamyss);

        auto kyrssa = new Model(BASESIZE, wounds());
        kyrssa->addMeleeWeapon(&m_sacrificialWeapons);
        kyrssa->setName("Kyrssa");
        addModel(kyrssa);

        auto lethyr = new Model(BASESIZE, wounds());
        lethyr->addMeleeWeapon(&m_sacrificialWeapons);
        lethyr->setName("Lethyr");
        addModel(lethyr);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *TheBladeCoven::Create(const ParameterList &parameters) {
        auto unit = new TheBladeCoven();
        unit->setTemple(Temple::HaggNar);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TheBladeCoven::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("The Blade Coven", factoryMethod);
        }
    }

    Wounds TheBladeCoven::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Heartseekers
        if ((hitRoll >= 6) && (weapon->name() == m_heartseekerBow.name())) {
            return {0, 1};
        }

        return DaughterOfKhaine::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int TheBladeCoven::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace DaughtersOfKhaine