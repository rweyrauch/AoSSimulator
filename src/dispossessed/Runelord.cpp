/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Runelord.h>
#include <UnitFactory.h>
#include "DispossessedPrivate.h"

namespace Dispossessed {
    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool Runelord::s_registered = false;

    Runelord::Runelord() :
            Dispossessed("Runelord", 4, g_wounds, 7, 4, false),
            m_runeStaff(Weapon::Type::Melee, "Rune Staff", 1, 1, 4, 3, 0, RAND_D3),
            m_forgehammer(Weapon::Type::Melee, "Forgehammer", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HERO, PRIEST, RUNELORD};
        m_weapons = {&m_runeStaff, &m_forgehammer};

        // Runes of Spellbreaking
        m_totalUnbinds = 1;
    }

    bool Runelord::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_runeStaff);
        model->addMeleeWeapon(&m_forgehammer);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Runelord::Create(const ParameterList &parameters) {
        auto unit = new Runelord();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Runelord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Runelord::Create,
                    Dispossessed::ValueToString,
                    Dispossessed::EnumStringToInt,
                    Runelord::ComputePoints,
                    {
                            EnumParameter("Grudge", g_grudge[0], g_grudge),
                            BoolParameter("General")
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Runelord", factoryMethod);
        }
    }

    int Runelord::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Dispossessed