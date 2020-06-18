/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukWarchanter.h>
#include <UnitFactory.h>
#include <Board.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    bool OrrukWarchanter::s_registered = false;

    OrrukWarchanter::OrrukWarchanter() :
            Ironjawz("Orruk Warchanter", 4, WOUNDS, 7, 4, false),
            m_stikks(Weapon::Type::Melee, "Gorkstikk and Morkstikk", 1, 6, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, HERO, TOTEM, WARCHANTER};
        m_weapons = {&m_stikks};
    }

    bool OrrukWarchanter::configure(Warbeat warbeat) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_stikks);
        addModel(model);

        m_warbeat = warbeat;
        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *OrrukWarchanter::Create(const ParameterList &parameters) {
        auto unit = new OrrukWarchanter();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto beat = (Warbeat) GetEnumParam("Warbeat", parameters, g_warbeats[0]);

        bool ok = unit->configure(beat);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void OrrukWarchanter::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukWarchanter::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    OrrukWarchanter::ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                             EnumParameter("Warbeat", g_warbeats[0], g_warbeats)
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Warchanter", factoryMethod);
        }
    }

    int OrrukWarchanter::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Ironjawz
