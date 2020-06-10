/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/SavageBigBoss.h"

namespace Bonesplitterz {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    bool SavageBigBoss::s_registered = false;

    Unit *SavageBigBoss::Create(const ParameterList &parameters) {
        auto unit = new SavageBigBoss();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, Bonegrinz);
        unit->setWarclan(warclan);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SavageBigBoss::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Savage Big Boss", factoryMethod);
        }
    }

    SavageBigBoss::SavageBigBoss() :
            Bonesplitterz("Savage Big Boss", 5, WOUNDS, 7, 6, false),
            m_chompasBoss(Weapon::Type::Melee, "Boss Chompa", 1, 6, 3, 3, -1, 2) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, SAVAGE_BIG_BOSS};
        m_weapons = {&m_chompasBoss};
    }

    bool SavageBigBoss::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_chompasBoss);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int SavageBigBoss::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Bonesplitterz
