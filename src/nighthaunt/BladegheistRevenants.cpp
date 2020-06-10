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
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 320;


    bool BladegheistRevenants::s_registered = false;

    BladegheistRevenants::BladegheistRevenants() :
            Nighthaunt("Bladegheist Revenants", 8, WOUNDS, 10, 4, true),
            m_tombGreatblade(Weapon::Type::Melee, "Tomb Greatblade", 1, 2, 3, 3, -1, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, BLADEGHEIST_REVENANTS};
        m_weapons = {&m_tombGreatblade};
    }

    bool BladegheistRevenants::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_retreatAndCharge = true;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_tombGreatblade);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *BladegheistRevenants::Create(const ParameterList &parameters) {
        auto unit = new BladegheistRevenants();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

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
                    nullptr,
                    nullptr,
                    BladegheistRevenants::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
                return RerollFailed;
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
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Nighthaunt
