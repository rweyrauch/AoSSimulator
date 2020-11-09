/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/PlagueCenserBearers.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Skaven {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 240;

    bool PlagueCenserBearers::s_registered = false;

    Unit *PlagueCenserBearers::Create(const ParameterList &parameters) {
        auto unit = new PlagueCenserBearers();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int PlagueCenserBearers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void PlagueCenserBearers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Plague Censer Bearers", factoryMethod);
        }
    }

    PlagueCenserBearers::PlagueCenserBearers() :
            Skaventide("Plague Censer Bearers", 6, g_wounds, 5, 6, false),
            m_censer(Weapon::Type::Melee, "Plague Censer", 2, 2, 4, 3, -1, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, NURGLE, CLANS_PESTILENS, PLAGUE_CENSER_BEARERS};
        m_weapons = {&m_censer};
    }

    bool PlagueCenserBearers::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_censer);
            addModel(model);
        }
        m_points = ComputePoints(numModels);

        return true;
    }

    int PlagueCenserBearers::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Skaventide::extraAttacks(attackingModel, weapon, target);

        // Frenzied Assault
        if (m_charged) extra++;

        return extra;
    }

    Rerolls PlagueCenserBearers::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Plaque Disciples
        auto monks = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), PLAGUE_MONKS, 18.0);
        if (monks) return Reroll_Failed;

        return Unit::toHitRerolls(weapon, target);
    }

    Rerolls PlagueCenserBearers::battleshockRerolls() const {
        // Plaque Disciples
        auto monks = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), PLAGUE_MONKS, 18.0);
        if (monks) return Reroll_Failed;

        return Unit::battleshockRerolls();
    }

    Wounds PlagueCenserBearers::onEndCombat(PlayerId player) {
        Wounds wounds = Unit::onEndCombat(player);

        // Poisonous Fumes
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        for (auto unit : units) {
            if (!unit->hasKeyword(CLANS_PESTILENS)) {
                int mortalWounds = 0;
                int roll = Dice::RollD6();
                if (roll == 6) mortalWounds = Dice::RollD3();
                else if (roll >= 4) mortalWounds = 1;

                unit->applyDamage({0, mortalWounds});
                wounds.mortal += mortalWounds;
            }
        }
        return wounds;
    }

} //namespace Skaven
