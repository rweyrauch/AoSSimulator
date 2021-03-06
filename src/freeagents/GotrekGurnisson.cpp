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
    static const int g_basesize = 32;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 520;

    bool GotrekGurnisson::s_registered = false;

    GotrekGurnisson::GotrekGurnisson(bool isGeneral) :
            Unit("Gotrek Gurnisson", 4, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_zangromThaz(Weapon::Type::Melee, "Zangrom-Thaz", 1, 6, 3, 3, -2, 3) {
        m_keywords = {ORDER, DUARDIN, HERO, GOTREK_GURNISSON};
        m_weapons = {&m_zangromThaz};

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_zangromThaz);
        addModel(model);
    }

    Unit *GotrekGurnisson::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new GotrekGurnisson(general);
    }

    void GotrekGurnisson::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GotrekGurnisson::Create,
                    nullptr,
                    nullptr,
                    GotrekGurnisson::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {FREE_AGENT}
            };
            s_registered = UnitFactory::Register("Gotrek Gurnisson", factoryMethod);
        }
    }

    Wounds GotrekGurnisson::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Avatar of Grimnir
        Wounds appliedWounds = {0, 0};
        appliedWounds.normal = std::min(1, wounds.normal);
        appliedWounds.mortal = std::min(1, wounds.mortal);

        // Should Plate of Edassa
        if (appliedWounds.normal > 0) {
            if (Dice::RollD6() >= 3) {
                appliedWounds.normal = 0;
            }
        }
        if (appliedWounds.mortal > 0) {
            if (Dice::RollD6() >= 3) {
                appliedWounds.mortal = 0;
            }
        }
        return appliedWounds;
    }

    Rerolls GotrekGurnisson::toHitRerolls(const Weapon * /*weapon*/, const Unit * /*target*/) const {
        // Krag Blackhammer's Master Rune
        return Rerolls::Failed;
    }

    Rerolls GotrekGurnisson::toWoundRerolls(const Weapon * /*weapon*/, const Unit * /*target*/) const {
        // Krag Blackhammer's Master Rune
        return Rerolls::Failed;
    }

    Wounds GotrekGurnisson::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        Wounds wounds = Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        // Krag Blackhammer's Master Rune
        if (hitRoll >= 6) {
            wounds.mortal += Dice::RollD6();
        }
        return wounds;
    }

    void GotrekGurnisson::onEndCombat(PlayerId player) {
        Unit *meleeTarget = m_meleeTarget;
        if (!meleeTarget) {
            meleeTarget = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        }

        Wounds wounds = {0, 0};

        // Unstoppable Battle Fury
        auto totalMoveDistance = distanceTo(meleeTarget);
        if (meleeTarget && (totalMoveDistance <= 3.0)) {
            Math::Ray ray(position(), meleeTarget->position());
            auto newPos = ray.pointAt(totalMoveDistance);
            move(newPos, ray.getDirection());

            int numSlain = 0;
            wounds += fight(-1, meleeTarget, numSlain);
            if (wounds.normal) {
                PLOG_INFO.printf("Gotrek attacks again for {%d, %d} wounds.", wounds.normal, wounds.mortal);
            }
        }
        Unit::onEndCombat(player);
    }

    int GotrekGurnisson::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }


} // namespace Fyreslayers
