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

namespace GloomspiteGitz {
    static const int BASESIZE = 25;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 6;
    static const int MAX_UNIT_SIZE = 24;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    bool SquiqHerd::s_registered = false;

    SquiqHerd::SquiqHerd() :
            GloomspiteGitzBase("Squig Herd", 5, WOUNDS, 3, 6, false),
            m_fangFilledGob(Weapon::Type::Melee, "Fang-filled Gob", 1, 2, 4, 3, -1, 1),
            m_squigProdder(Weapon::Type::Melee, "Squig Prodder", 1, 2, 5, 5, 0, 1) {
        m_keywords = {DESTRUCTION, SQUIG, GLOOMSPITE_GITZ, MOONCLAN, SQUIG_HERD};
        m_weapons = {&m_fangFilledGob, &m_squigProdder};
    }

    bool SquiqHerd::configure(int numModels) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        const int numHerders = (numModels + 5) / 6;

        // Add the herder
        for (auto i = 0; i < numHerders; i++) {
            auto herder = new Model(BASESIZE, wounds());
            herder->addMeleeWeapon(&m_squigProdder);
            addModel(herder);
        }

        // and the squigs
        for (auto i = numHerders; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_fangFilledGob);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SquiqHerd::Create(const ParameterList &parameters) {
        auto unit = new SquiqHerd();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SquiqHerd::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SquiqHerd::Create,
                    nullptr,
                    nullptr,
                    SquiqHerd::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE}
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
            return RerollFailed;
        }

        return GloomspiteGitzBase::runRerolls();
    }

    Rerolls SquiqHerd::chargeRerolls() const {
        // Go Dat Way!
        if (hasHerder()) {
            return RerollFailed;
        }

        return GloomspiteGitzBase::chargeRerolls();
    }

    bool SquiqHerd::hasHerder() const {
        // TODO: check for herders - assuming herder is always the last model removed.
        return true;
    }

    void SquiqHerd::onFlee(int numFled) {
        // Squigs Go Wild
        Dice::RollResult rolls;
        Dice::rollD6(numFled, rolls);
        int numMortalWounds = rolls.rollsGE(4);
        if (numMortalWounds) {
            auto board = Board::Instance();
            auto closestTarget = board->getNearestUnit(this, PlayerId::None);
            if (closestTarget && distanceTo(closestTarget) <= 6.0f) {
                closestTarget->applyDamage({0, numMortalWounds});
            }
        }
        GloomspiteGitzBase::onFlee(numFled);
    }

    int SquiqHerd::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace GloomspiteGitz
