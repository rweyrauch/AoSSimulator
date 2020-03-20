/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <freeagents/GotrekGurnisson.h>
#include <Board.h>
#include <UnitFactory.h>

namespace FreeAgent {
    static const int BASESIZE = 32;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 520;

    bool GotrekGurnisson::s_registered = false;

    GotrekGurnisson::GotrekGurnisson() :
            Unit("Gotrek Gurnisson", 4, WOUNDS, 10, 4, false),
            m_zangromThaz(Weapon::Type::Melee, "Zangrom-Thaz", 1, 6, 3, 3, -2, 3) {
        m_keywords = {ORDER, DUARDIN, HERO, GOTREK_GURNISSON};
        m_weapons = {&m_zangromThaz};
    }

    bool GotrekGurnisson::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_zangromThaz);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *GotrekGurnisson::Create(const ParameterList &parameters) {
        auto unit = new GotrekGurnisson();
        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GotrekGurnisson::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GotrekGurnisson::Create,
                    nullptr,
                    nullptr,
                    GotrekGurnisson::ComputePoints,
                    {
                    },
                    ORDER,
                    {FREE_AGENT}
            };
            s_registered = UnitFactory::Register("Gotrek Gurnisson", factoryMethod);
        }
    }

    Wounds GotrekGurnisson::applyWoundSave(const Wounds &wounds) {
        // Avatar of Grimnir
        Wounds appliedWounds = {0, 0};
        appliedWounds.normal = std::min(1, wounds.normal);
        appliedWounds.mortal = std::min(1, wounds.mortal);

        // Should Plate of Edassa
        if (appliedWounds.normal > 0) {
            if (Dice::rollD6() >= 3) {
                appliedWounds.normal = 0;
            }
        }
        if (appliedWounds.mortal > 0) {
            if (Dice::rollD6() >= 3) {
                appliedWounds.mortal = 0;
            }
        }
        return appliedWounds;
    }

    Rerolls GotrekGurnisson::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Krag Blackhammer's Master Rune
        return RerollFailed;
    }

    Rerolls GotrekGurnisson::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Krag Blackhammer's Master Rune
        return RerollFailed;
    }

    Wounds GotrekGurnisson::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        Wounds wounds = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        // Krag Blackhammer's Master Rune
        if (hitRoll >= 6) {
            wounds.mortal += Dice::rollD6();
        }
        return wounds;
    }

    Wounds GotrekGurnisson::onEndCombat(PlayerId player) {
        Unit *meleeTarget = m_meleeTarget;
        if (!meleeTarget) {
            meleeTarget = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        }

        Wounds wounds = {0, 0};

        // Unstoppable Battle Fury
        auto totalMoveDistance = distanceTo(meleeTarget);
        if (meleeTarget && (totalMoveDistance <= 3.0f)) {
            Math::Ray ray(position(), meleeTarget->position());
            auto newPos = ray.point_at(totalMoveDistance);
            setPosition(newPos, ray.get_direction());

            int numSlain = 0;
            wounds += fight(-1, meleeTarget, numSlain);
            if (wounds.normal) {
                SimLog(Verbosity::Narrative, "Gotrek attacks again for {%d, %d} wounds.", wounds.normal, wounds.mortal);
            }
        }

        wounds += Unit::onEndCombat(player);

        return wounds;
    }

    int GotrekGurnisson::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }


} // namespace Fyreslayers
