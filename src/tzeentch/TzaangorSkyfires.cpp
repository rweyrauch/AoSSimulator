/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/TzaangorSkyfires.h>
#include <UnitFactory.h>
#include <Board.h>
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 9;
    static const int g_pointsPerBlock = 200;
    static const int g_pointsMaxUnitSize = 600;

    bool TzaangorSkyfires::s_registered = false;

    TzaangorSkyfires::TzaangorSkyfires(int points) :
            TzeentchBase("Tzaangor Skyfires", 16, g_wounds, 6, 5, true, points),
            m_arrowOfFate(Weapon::Type::Missile, "Arrow of Fate", 24, 1, 4, 3, -1, RAND_D3),
            m_arrowOfFateAviarch(Weapon::Type::Missile, "Arrow of Fate", 24, 1, 3, 3, -1, RAND_D3),
            m_bowStave(Weapon::Type::Melee, "Bow Stave", 1, 2, 5, 5, 0, 1),
            m_viciousBeak(Weapon::Type::Melee, "Vicious Beak", 1, 1, 4, 5, 0, 1),
            m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, TZEENTCH, ARCANITE, TZAANGOR_SKYFIRES};
        m_weapons = {&m_arrowOfFate, &m_arrowOfFateAviarch, &m_bowStave, &m_viciousBeak, &m_teethAndHorns};
        m_hasMount = true;
        m_teethAndHorns.setMount(true);
    }

    bool TzaangorSkyfires::configure(int numModels) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        auto aviarch = new Model(g_basesize, wounds());
        aviarch->addMissileWeapon(&m_arrowOfFateAviarch);
        aviarch->addMeleeWeapon(&m_bowStave);
        aviarch->addMeleeWeapon(&m_viciousBeak);
        aviarch->addMeleeWeapon(&m_teethAndHorns);
        addModel(aviarch);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_arrowOfFate);
            model->addMeleeWeapon(&m_bowStave);
            model->addMeleeWeapon(&m_viciousBeak);
            model->addMeleeWeapon(&m_teethAndHorns);
            addModel(model);
        }

        return true;
    }

    Unit *TzaangorSkyfires::Create(const ParameterList &parameters) {
        auto *unit = new TzaangorSkyfires(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TzaangorSkyfires::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TzaangorSkyfires::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    TzaangorSkyfires::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH, BEASTS_OF_CHAOS}
            };
            s_registered = UnitFactory::Register("Tzaangor Skyfires", factoryMethod);
        }
    }

    Rerolls TzaangorSkyfires::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Guided by the Future
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        bool enemyHasFought = false;
        for (auto ip : units) {
            enemyHasFought |= ip->hasFought();
        }
        if (!enemyHasFought) return Rerolls::Failed;

        return TzeentchBase::toHitRerolls(weapon, target);
    }

    Rerolls TzaangorSkyfires::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Guided by the Future
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        bool enemyHasFought = false;
        for (auto ip : units) {
            enemyHasFought |= ip->hasFought();
        }
        if (!enemyHasFought) return Rerolls::Failed;

        return TzeentchBase::toWoundRerolls(weapon, target);
    }

    Wounds TzaangorSkyfires::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Judgement from Afar
        if ((hitRoll == 6) && (weapon->name() == m_arrowOfFate.name())) {
            return {0, Dice::RollD3()};
        }
        return TzeentchBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int TzaangorSkyfires::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Tzeentch