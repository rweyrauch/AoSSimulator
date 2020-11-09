/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "kharadron/ThundriksProfiteers.h"

namespace KharadronOverlords {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 0;

    bool ThundriksProfiteers::s_registered = false;

    Unit *ThundriksProfiteers::Create(const ParameterList &parameters) {
        auto unit = new ThundriksProfiteers();

        unit->setSkyport(Skyport::Barak_Nar);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int ThundriksProfiteers::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void ThundriksProfiteers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ThundriksProfiteers::Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    ThundriksProfiteers::ComputePoints,
                    {},
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Thundrik's Profiteers", factoryMethod);
        }
    }

    ThundriksProfiteers::ThundriksProfiteers() :
            KharadronBase("Thundrik's Profiteers", 4, g_wounds, 7, 4, false),
            m_volleyGun(Weapon::Type::Missile, "Aethermatic Volley Gun", 18, 6, 5, 4, -1, 1),
            m_rifle(Weapon::Type::Missile, "Aethershot Rifle", 18, 4, 3, 4, -1, 1),
            m_privateerPistol(Weapon::Type::Missile, "Privateer Pistol", 12, 2, 4, 4, 0, 1),
            m_vulcaniserPistol(Weapon::Type::Missile, "Vulcaniser Pistol", 9, 2, 3, 3, -1, 1),
            m_cutter(Weapon::Type::Melee, "Arkanaut Cutter", 1, 2, 4, 4, 0, 1),
            m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 4, 0, 1),
            m_skypike(Weapon::Type::Melee, "Skybite", 2, 2, 4, 3, -1, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, BARAK_NAR, SKYFARER,
                      MARINE, THUNDRIKS_PROFITEERS};
        m_weapons = {&m_volleyGun, &m_rifle, &m_privateerPistol, &m_vulcaniserPistol, &m_cutter,
                     &m_gunButt, &m_skypike};
    }

    bool ThundriksProfiteers::configure() {

        auto garodd = new Model(g_basesize, wounds());
        garodd->addMissileWeapon(&m_privateerPistol);
        garodd->addMeleeWeapon(&m_cutter);
        garodd->setName("Garodd Alensen");
        addModel(garodd);

        auto enrik = new Model(g_basesize, wounds());
        enrik->addMissileWeapon(&m_volleyGun);
        enrik->addMeleeWeapon(&m_gunButt);
        enrik->setName("Enrik Ironhail");
        addModel(enrik);

        auto deadeye = new Model(g_basesize, wounds());
        deadeye->addMissileWeapon(&m_rifle);
        deadeye->addMeleeWeapon(&m_gunButt);
        deadeye->setName("Dead-Eye Lund");
        addModel(deadeye);

        auto khazgan = new Model(g_basesize, wounds());
        khazgan->addMissileWeapon(&m_vulcaniserPistol);
        khazgan->addMeleeWeapon(&m_skypike);
        khazgan->setName("Khazgan Drakkskewer");
        addModel(khazgan);

        m_points = ThundriksProfiteers::ComputePoints(1);

        return true;
    }

} // namespace Kharadron
