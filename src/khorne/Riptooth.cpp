/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Riptooth.h>
#include <UnitFactory.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_pointsPerUnit = 40;

    bool Riptooth::s_registered = false;

    Unit *Riptooth::Create(const ParameterList &parameters) {
        auto unit = new Riptooth();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        unit->configure();
        return unit;
    }

    void Riptooth::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Riptooth::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Riptooth::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Riptooth", factoryMethod);
        }
    }

    Riptooth::Riptooth() :
            KhorneBase("Riptooth", 8, g_wounds, 10, 5, false),
            m_claws(Weapon::Type::Melee, "Blood-dark Claws", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, GORETIDE, KHORNE, FLESH_HOUND, RIPTOOTH};
        m_weapons = {&m_claws};

        // Collar of Khorne
        m_totalUnbinds = 1;
    }

    void Riptooth::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    int Riptooth::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Khorne
