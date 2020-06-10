/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/RatOgors.h>
#include <UnitFactory.h>

namespace Skaven {
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 8;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    bool RatOgors::s_registered = false;

    Unit *RatOgors::Create(const ParameterList &parameters) {
        auto unit = new RatOgors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        int numGuns = GetIntParam("Warpfire Guns", parameters, MIN_UNIT_SIZE / 2);

        bool ok = unit->configure(numModels, numGuns);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int RatOgors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    void RatOgors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            IntegerParameter("Warpfire Guns", MIN_UNIT_SIZE / 2, MIN_UNIT_SIZE / 2,
                             MAX_UNIT_SIZE / 2, MIN_UNIT_SIZE / 2),
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Rat Ogors", factoryMethod);
        }
    }

    RatOgors::RatOgors() :
            Skaventide("Rat Ogors", 6, WOUNDS, 5, 5, false),
            m_gun(Weapon::Type::Missile, "Warpfire Gun", 16, 1, 5, 3, -1, RAND_D3),
            m_clawsBladesAndFangs(Weapon::Type::Melee, "Tearing Claws, Blades and Fangs", 1, 4, 4, 3, -1, 2) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_MOULDER, FIGHTING_BEAST, PACK, RAT_OGORS};
        m_weapons = {&m_gun, &m_clawsBladesAndFangs};
    }

    bool RatOgors::configure(int numModels, int numGuns) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }
        const int maxGuns = numModels / 2;
        if (numGuns > maxGuns) {
            return false;
        }

        for (auto i = 0; i < numGuns; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_gun);
            model->addMeleeWeapon(&m_clawsBladesAndFangs);
            addModel(model);
        }
        for (auto i = numGuns; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_clawsBladesAndFangs);
            addModel(model);
        }
        m_points = ComputePoints(numModels);

        return true;
    }

    int RatOgors::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Rabid Fury
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_clawsBladesAndFangs.name())) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

} //namespace Skaven
