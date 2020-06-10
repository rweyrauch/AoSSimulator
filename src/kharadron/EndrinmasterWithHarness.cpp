/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/EndrinmasterWithHarness.h>
#include <UnitFactory.h>

namespace KharadronOverlords {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    bool EndrinmasterWithEndrinharness::s_registered = false;

    Unit *EndrinmasterWithEndrinharness::Create(const ParameterList &parameters) {
        auto unit = new EndrinmasterWithEndrinharness();

        auto port = (Skyport) GetEnumParam("Skyport", parameters, KharadronOverlords::Custom);
        unit->setSkyport(port);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void EndrinmasterWithEndrinharness::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    EndrinmasterWithEndrinharness::Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    EndrinmasterWithEndrinharness::ComputePoints,
                    {
                            EnumParameter("Skyport", g_skyport[0], g_skyport)
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Endrinmaster with Endrinharness", factoryMethod);
        }
    }

    EndrinmasterWithEndrinharness::EndrinmasterWithEndrinharness() :
            KharadronBase("Endrinmaster with Endrinharness", 4, WOUNDS, 7, 4, false),
            m_gaze(Weapon::Type::Missile, "Gaze of Grungni", 9, 1, 3, 2, -1, RAND_D3),
            m_hammer(Weapon::Type::Melee, "Aethermight Hammer", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, HERO, SKYFARER, MARINE, ENDRINMASTER};
        m_weapons = {&m_gaze, &m_hammer};
    }

    bool EndrinmasterWithEndrinharness::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_gaze);
        model->addMeleeWeapon(&m_hammer);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int EndrinmasterWithEndrinharness::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //KharadronOverlords
