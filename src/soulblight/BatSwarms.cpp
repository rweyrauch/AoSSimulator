/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/BatSwarms.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 50;
    static const int g_wounds = 5;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 8;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 320;

    bool BatSwarms::s_registered = false;

    Unit *BatSwarms::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new BatSwarms(legion, numModels, ComputePoints(parameters));
    }

    int BatSwarms::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void BatSwarms::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Legion", g_legions[0], g_legions)
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Bat Swarms", factoryMethod);
        }
    }

    BatSwarms::BatSwarms(Legion legion, int numModels, int points) :
            SoulblightBase(legion, "Bat Swarms", 12, g_wounds, 10, NoSave, true, points),
            m_teeth(Weapon::Type::Melee, "Razor-sharp Teeth", 3, 5, 5, 5, 0, 1) {
        m_keywords = {DEATH, SOULBLIGHT, SUMMONABLE, BAT_SWARMS};
        m_weapons = {&m_teeth};

        s_globalToHitMod.connect(this, &BatSwarms::cloudOfHorrors, &m_cloudSlot);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_teeth);
            addModel(model);
        }
    }

    BatSwarms::~BatSwarms() {
        m_cloudSlot.disconnect();
    }

    int BatSwarms::cloudOfHorrors(const Unit * /*attacker*/, const Weapon *weapon, const Unit *target) {
        if (!isFriendly(target) && weapon->isMissile() && (distanceTo(target) <= 12.0)) return -1;
        return 0;
    }

} // namespace Soulblight
