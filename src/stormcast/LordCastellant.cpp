/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordCastellant.h>
#include <iostream>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    bool LordCastellant::s_registered = false;

    LordCastellant::LordCastellant() :
            StormcastEternal("Lord-Castellant", 5, WOUNDS, 9, 3, false),
            m_halberd(Weapon::Type::Melee, "Castellant's Halberd", 2, 3, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, LORD_CASTELLANT};
        m_weapons = {&m_halberd};
    }

    bool LordCastellant::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_halberd);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *LordCastellant::Create(const ParameterList &parameters) {
        auto unit = new LordCastellant();

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordCastellant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Castellant", factoryMethod);
        }
    }

    int LordCastellant::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals