/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "slavestodarkness/MindstealerSphiranx.h"
#include "SlavesToDarknessPrivate.h"


namespace SlavesToDarkness {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 100;

    bool MindstealerSphiranx::s_registered = false;

    Unit *MindstealerSphiranx::Create(const ParameterList &parameters) {
        auto unit = new MindstealerSphiranx();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MindstealerSphiranx::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };

            s_registered = UnitFactory::Register("Mindstealer Sphiranx", factoryMethod);
        }
    }

    MindstealerSphiranx::MindstealerSphiranx() :
            SlavesToDarknessBase("Mindstealer Sphiranx", 10, WOUNDS, 10, 5, false),
            m_claws(Weapon::Type::Melee, "Shredding Claws", 1, 3, 3, 3, -1, 1),
            m_tail(Weapon::Type::Melee, "Lashing Tail", 1, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, MONSTER, SLAVES_TO_DARKNESS, MINDSTEALER_SPHIRANX};

        s_globalBraveryMod.connect(this, &MindstealerSphiranx::telepathicDread, &m_connection);
    }

    MindstealerSphiranx::~MindstealerSphiranx() {
        m_connection.disconnect();
    }

    bool MindstealerSphiranx::configure() {
        auto model = new Model(BASESIZE, wounds());

        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int MindstealerSphiranx::telepathicDread(const Unit *unit) {
        // Telepathic Dread
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 12.0)) {
            return -2;
        }

        return 0;
    }

    int MindstealerSphiranx::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

}//namespace SlavesToDarkness
