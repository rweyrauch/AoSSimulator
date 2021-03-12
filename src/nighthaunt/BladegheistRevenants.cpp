/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/BladegheistRevenants.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>

namespace Nighthaunt {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 90;
    static const int g_pointsMaxUnitSize = 320;


    bool BladegheistRevenants::s_registered = false;

    BladegheistRevenants::BladegheistRevenants() :
            Nighthaunt("Bladegheist Revenants", 8, g_wounds, 10, 4, true),
            m_tombGreatblade(Weapon::Type::Melee, "Tomb Greatblade", 1, 2, 3, 3, -1, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, BLADEGHEIST_REVENANTS};
        m_weapons = {&m_tombGreatblade};
    }

    bool BladegheistRevenants::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_retreatAndCharge = true;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_tombGreatblade);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BladegheistRevenants::Create(const ParameterList &parameters) {
        auto unit = new BladegheistRevenants();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BladegheistRevenants::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BladegheistRevenants::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    BladegheistRevenants::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Bladegheist Revenants", factoryMethod);
        }
    }

    Rerolls BladegheistRevenants::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Fearful Frenzy
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
        for (auto ip : units) {
            if (ip->hasKeyword(SPIRIT_TORMENT) || ip->hasKeyword(CHAINGHASTS)) {
                return Rerolls::Failed;
            }
        }

        return Nighthaunt::toHitRerolls(weapon, unit);
    }

    int
    BladegheistRevenants::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // Whirling Death
        int attacks = Nighthaunt::extraAttacks(attackingModel, weapon, target);
        if (m_charged) {
            attacks += 1;
        }
        return attacks;
    }

    int BladegheistRevenants::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nighthaunt
