/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Scraplauncher.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 120;

    bool GnoblarScraplauncher::s_registered = false;

    bool GnoblarScraplauncher::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *GnoblarScraplauncher::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            return new GnoblarScraplauncher(tribe);
        }
        return nullptr;
    }

    void GnoblarScraplauncher::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GnoblarScraplauncher::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    GnoblarScraplauncher::ComputePoints,
                    {
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Gnoblar Scraplauncher", factoryMethod);
        }
    }

    GnoblarScraplauncher::GnoblarScraplauncher(Mawtribe tribe) :
            MawtribesBase(tribe, "Gnoblar Scraplauncher", 7, g_wounds, 5, 4, false) {
        m_keywords = {DESTRUCTION, GROT, RHINOX, OGOR_MAWTRIBES, GUTBUSTERS, GNOBLAR_SCRAPLAUNCHER};
        m_weapons = {&m_scrap, &m_scrapperWeapons, &m_horns};
        m_battleFieldRole = Role::Artillery;

        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_scrap);
        model->addMeleeWeapon(&m_scrapperWeapons);
        model->addMeleeWeapon(&m_horns);

        addModel(model);

        m_points = GnoblarScraplauncher::ComputePoints(1);
    }

    int GnoblarScraplauncher::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Deadly Rain of Scrap
        if ((weapon->name() == m_scrap.name()) && (target->remainingModels() >= 10)) {
            mod++;
        }

        return mod;
    }

    Wounds
    GnoblarScraplauncher::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deadly Rain of Scrap
        if ((weapon->name() == m_scrap.name()) && (target->remainingModels() >= 10)) {
            return {RAND_D6, 0};
        }
            // Rhinox Charge
        else if ((weapon->name() == m_horns.name()) && m_charged) {
            return {weapon->damage() + 1, 0};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int GnoblarScraplauncher::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
