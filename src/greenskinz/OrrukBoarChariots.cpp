/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/OrrukBoarChariots.h>
#include <UnitFactory.h>

namespace Greenskinz {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 6;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 240;

    bool OrrukBoarChariots::s_registered = false;

    OrrukBoarChariots::OrrukBoarChariots(int points) :
            Unit("Orruk Boar Chariots", 9, g_wounds, 6, 4, false, points),
            m_pigstikkaSpears(Weapon::Type::Melee, "Crew's Pigstikka Spears", 2, 2, 4, 4, 0, 1),
            m_warBoarsTusks(Weapon::Type::Melee, "War Boar's Tusks", 1, 4, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, ORRUK_BOAR_CHARIOTS};
        m_weapons = {&m_pigstikkaSpears, &m_warBoarsTusks};
        m_hasMount = true;
        m_warBoarsTusks.setMount(true);
    }

    bool OrrukBoarChariots::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_pigstikkaSpears);
            model->addMeleeWeapon(&m_warBoarsTusks);
            addModel(model);
        }

        return true;
    }

    Unit *OrrukBoarChariots::Create(const ParameterList &parameters) {
        auto unit = new OrrukBoarChariots(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void OrrukBoarChariots::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukBoarChariots::Create,
                    nullptr,
                    nullptr,
                    OrrukBoarChariots::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    DESTRUCTION,
                    {GREENSKINZ}
            };
            s_registered = UnitFactory::Register("Orruk Boar Chariots", factoryMethod);
        }
    }

    int OrrukBoarChariots::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        // Tusker Charge
        if (m_charged && weapon->name() == m_warBoarsTusks.name()) {
            mod++;
        }
        return mod;
    }

    void OrrukBoarChariots::onCharged() {
        // Scythed Wheels
        int roll = Dice::RollD6();
        if (roll >= 4) {
            if (m_meleeTarget) {
                m_meleeTarget->applyDamage({0, Dice::RollD3()}, this);
            }
        }
        Unit::onCharged();
    }

    int OrrukBoarChariots::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Greenskinz
