/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/BrokkGrungsson.h>
#include <UnitFactory.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 40;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 220;

    bool BrokkGrungsson::s_registered = false;

    Unit *BrokkGrungsson::Create(const ParameterList &parameters) {
        auto unit = new BrokkGrungsson();

        unit->setSkyport(Skyport::Barak_Nar);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BrokkGrungsson::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BrokkGrungsson::Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    BrokkGrungsson::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Brokk Grungsson", factoryMethod);
        }
    }

    BrokkGrungsson::BrokkGrungsson() :
            KharadronBase("Brokk Grungsson", 12, g_wounds, 8, 3, true),
            m_boast(Weapon::Type::Missile, "Grungsson's Boast", 18, 2, 3, 2, -2, RAND_D3),
            m_charter(Weapon::Type::Missile, "The Magnate's Charter", 18, 6, 3, 3, -1, 1),
            m_aetherblasters(Weapon::Type::Missile, "Aetherblasters", 9, 2, 3, 4, 0, 1),
            m_saw(Weapon::Type::Melee, "Aethermatic Saw", 1, 4, 3, 2, -2, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, BARAK_NAR, HERO, SKYFARER, BROKK_GRUNGSSON};
        m_weapons = {&m_boast, &m_charter, &m_aetherblasters, &m_saw};
        m_battleFieldRole = Leader;
    }

    bool BrokkGrungsson::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_boast);
        model->addMissileWeapon(&m_charter);
        model->addMissileWeapon(&m_aetherblasters);
        model->addMeleeWeapon(&m_saw);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int BrokkGrungsson::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //KharadronOverlords
