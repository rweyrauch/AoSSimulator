/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/SquigHerd.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 25;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 6;
    static const int g_maxUnitSize = 24;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 280;

    bool SquiqHerd::s_registered = false;

    SquiqHerd::SquiqHerd(Allegiance allegiance, int numModels, int points) :
            GloomspiteGitzBase(allegiance, "Squig Herd", 5, g_wounds, 3, 6, false, points),
            m_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1),
            m_squigProdder(Weapon::Type::Melee, "Squig Prodder", 1, 2, 5, 5, 0, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, SQUIG_HERD};
        m_weapons = {&m_fangFilledGob, &m_squigProdder};

        const int numHerders = (numModels + 5) / 6;

        // Add the herder
        for (auto i = 0; i < numHerders; i++) {
            auto herder = new Model(g_basesize, wounds());
            herder->addMeleeWeapon(&m_squigProdder);
            herder->setName("Herder");
            addModel(herder);
        }

        // and the squigs
        for (auto i = numHerders; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_fangFilledGob);
            addModel(model);
        }
    }

    Unit *SquiqHerd::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return new SquiqHerd(allegiance, numModels, ComputePoints(parameters));
    }

    void SquiqHerd::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SquiqHerd::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    SquiqHerd::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Squiq Herd", factoryMethod);
        }
    }

    Rerolls SquiqHerd::runRerolls() const {
        // Go Dat Way!
        if (hasHerder()) {
            return Rerolls::Failed;
        }

        return GloomspiteGitzBase::runRerolls();
    }

    Rerolls SquiqHerd::chargeRerolls() const {
        // Go Dat Way!
        if (hasHerder()) {
            return Rerolls::Failed;
        }

        return GloomspiteGitzBase::chargeRerolls();
    }

    bool SquiqHerd::hasHerder() const {
        return isNamedModelAlive("Herder");
    }

    void SquiqHerd::onFlee(int numFled) {
        // Squigs Go Wild
        Dice::RollResult rolls;
        Dice::RollD6(numFled, rolls);
        int numMortalWounds = rolls.rollsGE(4);
        if (numMortalWounds) {
            auto board = Board::Instance();
            auto closestTarget = board->getNearestUnit(this, PlayerId::None);
            if (closestTarget && distanceTo(closestTarget) <= 6.0) {
                closestTarget->applyDamage({0, numMortalWounds}, this);
            }
        }
        GloomspiteGitzBase::onFlee(numFled);
    }

    int SquiqHerd::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace GloomspiteGitz
