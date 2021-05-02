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
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 4;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 140;

    bool Chainghasts::s_registered = false;

    Chainghasts::Chainghasts(int numModels, int points) :
            Nighthaunt("Chainghasts", 6, g_wounds, 10, 4, true, points) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, SPIRIT_HOSTS};
        m_weapons = {&m_ghastflailsMissile, &m_ghastflails};

        s_globalToHitReroll.connect(this, &Chainghasts::anotherLinkInTheChain, &m_linkInTheChainSlot);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_ghastflailsMissile);
            model->addMeleeWeapon(&m_ghastflails);
            addModel(model);
        }
    }

    Chainghasts::~Chainghasts() {
        m_linkInTheChainSlot.disconnect();
    }

    Unit *Chainghasts::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new Chainghasts(numModels, ComputePoints(parameters));
    }

    void Chainghasts::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Chainghasts::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    Chainghasts::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
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
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 2.0);
            attacks += (int) units.size();
        }
        return attacks;
    }

    int Chainghasts::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls Chainghasts::anotherLinkInTheChain(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (isFriendly(attacker) && (distanceTo(attacker) < 12.0)) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit: units) {
                if ((unit->remainingModels() > 0) && unit->hasKeyword(SPIRIT_TORMENT) && (distanceTo(unit) < 12.0)) {
                    return Rerolls::Ones;
                }
            }
        }
        return Rerolls::None;
    }

} //namespace Nighthaunt
