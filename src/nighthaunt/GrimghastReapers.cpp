/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <nighthaunt/GrimghastReapers.h>
#include <UnitFactory.h>
#include <iostream>

namespace Nighthaunt {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 420;

    bool GrimghastReapers::s_registered = false;

    GrimghastReapers::GrimghastReapers() :
            Nighthaunt("Grimghast Reapers", 8, WOUNDS, 10, 4, true),
            m_slasherScythe(Weapon::Type::Melee, "Slasher Scythe", 2, 2, 4, 3, -1, 1),
            m_deathKnell(Weapon::Type::Melee, "Death Knell", 2, 1, 3, 3, -1, 2) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, GRIMGHAST_REAPERS};
        m_weapons = {&m_slasherScythe, &m_deathKnell};
    }

    bool GrimghastReapers::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto extoller = new Model(BASESIZE, wounds());
        // TODO: implement 'For Whom the Bell Tolls' (mortal wounds)
        extoller->addMeleeWeapon(&m_deathKnell);
        addModel(extoller);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_slasherScythe);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *GrimghastReapers::Create(const ParameterList &parameters) {
        auto unit = new GrimghastReapers();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GrimghastReapers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GrimghastReapers::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    GrimghastReapers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Grimghast Reapers", factoryMethod);
        }
    }

    Rerolls GrimghastReapers::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Reaped Like Corn
        if (weapon->name() == "Slasher Scythe" && unit->remainingModels() >= 5) {
            return RerollFailed;
        }

        return Nighthaunt::toHitRerolls(weapon, unit);
    }

    int GrimghastReapers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Nighthaunt
