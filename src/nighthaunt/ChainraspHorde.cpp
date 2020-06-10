/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/ChainraspHorde.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt {
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool ChainraspHorde::s_registered = false;

    ChainraspHorde::ChainraspHorde() :
            Nighthaunt("Chainrasp Horde", 6, WOUNDS, 10, 5, true), // todo: bravery 6 when no Dreadwarden
            m_malignantWeapon(Weapon::Type::Melee, "Malignant Weapon", 1, 2, 4, 4, 0, 1),
            m_malignantWeaponWarden(Weapon::Type::Melee, "Malignant Weapon", 1, 3, 4, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, CHAINRASP_HORDE};
        m_weapons = {&m_malignantWeapon, &m_malignantWeaponWarden};
    }

    bool ChainraspHorde::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto warden = new Model(BASESIZE, wounds());
        warden->addMeleeWeapon(&m_malignantWeaponWarden);
        addModel(warden);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_malignantWeapon);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *ChainraspHorde::Create(const ParameterList &parameters) {
        auto unit = new ChainraspHorde();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChainraspHorde::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChainraspHorde::Create,
                    nullptr,
                    nullptr,
                    ChainraspHorde::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };

            s_registered = UnitFactory::Register("Chainrasp Horde", factoryMethod);
        }
    }

    Rerolls ChainraspHorde::toWoundRerolls(const Weapon *weapon, const Unit *unit) const {
        // Chilling Horde
        if (remainingModels() >= 10) {
            return RerollOnes;
        }

        return Nighthaunt::toHitRerolls(weapon, unit);
    }

    int ChainraspHorde::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Nighthaunt
