/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "nurgle/LordOfAfflictions.h"

namespace Nurgle {
    static const int BASESIZE = 60;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 200;

    bool LordOfAfflictions::s_registered = false;

    Unit *LordOfAfflictions::Create(const ParameterList &parameters) {
        auto unit = new LordOfAfflictions();
        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordOfAfflictions::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LordOfAfflictions::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    LordOfAfflictions::ComputePoints,
                    {
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Lord of Afflictions", factoryMethod);
        }
    }

    LordOfAfflictions::LordOfAfflictions() :
            NurgleBase("Lord of Afflictions", 8, WOUNDS, 10, 4, true),
            m_festerspike(Weapon::Type::Melee, "Festerspike", 2, 3, 3, 3, -1, RAND_D3),
            m_mouthparts(Weapon::Type::Melee, "Foul Mouthparts", 1, 2, 3, 3, 0, 1),
            m_sting(Weapon::Type::Melee, "Venomous String", 1, 1, 4, 3, -1, RAND_D3),
            m_tocsin(Weapon::Type::Melee, "Dolorous Tocsin", 1, 1, 4, 3, -2, 2) {
        m_keywords = {CHAOS, MORTAL, DAEMON, NURGLE, ROTBRINGER, HERO, LORD_OF_AFFLICTIONS};
        m_weapons = {&m_festerspike, &m_mouthparts, &m_sting, &m_tocsin};

        s_globalToHitReroll.connect(this, &LordOfAfflictions::plagueVectorToHitRerolls, &m_plagueVectorSlot);
    }

    LordOfAfflictions::~LordOfAfflictions() {
        m_plagueVectorSlot.disconnect();
    }

    bool LordOfAfflictions::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_festerspike);
        model->addMeleeWeapon(&m_mouthparts);
        model->addMeleeWeapon(&m_sting);
        model->addMeleeWeapon(&m_tocsin);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds LordOfAfflictions::applyWoundSave(const Wounds &wounds) {
        // Disgustingly Resilient
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        return totalWounds.clamp();
    }

    void LordOfAfflictions::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            // Rotten Regeneration
            heal(1);

            auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0f);

            // Incubatch
            for (auto unit : units) {
                auto roll = Dice::rollD6();
                if (unit->hasKeyword(NURGLE) && (roll >= 6)) unit->applyDamage({0, 1});
                else if (roll >= 2) unit->applyDamage({0, 1});
            }

            // Virulent Discharge
            for (auto unit : units) {
                if (Dice::rollD6() >= 6) {
                    if (unit->hasKeyword(NURGLE)) unit->heal(Dice::rollD3());
                    else unit->applyDamage({0, Dice::rollD3()});
                }
            }
        }

    }

    Rerolls
    LordOfAfflictions::plagueVectorToHitRerolls(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (isFriendly(attacker) && attacker->hasKeyword(ROTBRINGER) && (distanceTo(attacker) <= 7.0f))
            return RerollOnes;

        return NoRerolls;
    }

    int LordOfAfflictions::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
