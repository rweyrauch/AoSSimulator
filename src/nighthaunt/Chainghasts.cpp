/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/Chainghasts.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Nighthaunt {
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 4;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 140;

    bool Chainghasts::s_registered = false;

    Chainghasts::Chainghasts() :
            Nighthaunt("Chainghasts", 6, WOUNDS, 10, 4, true),
            m_ghastflailsMissile(Weapon::Type::Missile, "Ghastflails (Missile)", 15, RAND_D3, 4, 3, -2, 1),
            m_ghastflails(Weapon::Type::Melee, "Ghastflails", 2, 0, 4, 3, -1, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, SPIRIT_HOSTS};
        m_weapons = {&m_ghastflailsMissile, &m_ghastflails};
    }

    bool Chainghasts::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_ghastflailsMissile);
            model->addMeleeWeapon(&m_ghastflails);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Chainghasts::Create(const ParameterList &parameters) {
        auto unit = new Chainghasts();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Chainghasts::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Chainghasts::Create,
                    nullptr,
                    nullptr,
                    Chainghasts::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };

            s_registered = UnitFactory::Register("Chainghasts", factoryMethod);
        }
    }

    int Chainghasts::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = Nighthaunt::extraAttacks(attackingModel, weapon, target);
        // Sweeping Blows
        if (weapon->name() == m_ghastflails.name()) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 2.0f);
            attacks += (int) units.size();
        }
        return attacks;
    }

    int Chainghasts::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace Nighthaunt
