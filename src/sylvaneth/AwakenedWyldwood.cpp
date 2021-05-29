/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "sylvaneth/AwakenedWyldwood.h"

namespace Sylvaneth {
    static const int g_basesize = 200;
    static const int g_pointsPerUnit = 0;

    bool AwakenedWyldwood::s_registered = false;

    Unit *AwakenedWyldwood::Create(const ParameterList &parameters) {
        return new AwakenedWyldwood();
    }

    void AwakenedWyldwood::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AwakenedWyldwood::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    AwakenedWyldwood::ComputePoints,
                    {
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Awakened Wyldwood", factoryMethod);
        }
    }

    AwakenedWyldwood::AwakenedWyldwood() :
            SylvanethBase("Awakened Wyldwood", 0, 0, Autopass_Battleshock, NoSave, false, g_pointsPerUnit) {
        m_keywords = {SCENERY, AWAKENED_WYLDWOOD};
        m_weapons = {&m_placeholder};

        auto woods = new Model(g_basesize, wounds());
        woods->addMeleeWeapon(&m_placeholder);
        addModel(woods);
    }

    int AwakenedWyldwood::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Sylvaneth