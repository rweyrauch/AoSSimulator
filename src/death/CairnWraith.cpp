/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/CairnWraith.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 25;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 60;

    bool CairnWraith::s_registered = false;

    Unit *CairnWraith::Create(const ParameterList &parameters) {
        auto unit = new CairnWraith();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int CairnWraith::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void CairnWraith::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Cairn Wraith", factoryMethod);
        }
    }

    CairnWraith::CairnWraith() :
            LegionOfNagashBase("Cairn Wraith", 6, WOUNDS, 10, 4, true),
            m_scythe(Weapon::Type::Melee, "Reaper Scythe", 2, 3, 4, 3, -1, 2) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, CAIRN_WRAITH};
        m_weapons = {&m_scythe};
    }

    bool CairnWraith::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_scythe);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }
} // namespace Death
