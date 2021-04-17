/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Jabberslythe.h>
#include <UnitFactory.h>
#include <Board.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 160;

    bool Jabberslythe::s_registered = false;

    Jabberslythe::Jabberslythe(Greatfray fray) :
            BeastsOfChaosBase("Jabberslythe", 12, g_wounds, 6, 5, true),
            m_slytheyTongue(Weapon::Type::Missile, "Slythey Tongue", 9, 1, 3, 3, -1, RAND_D3),
            m_vorpalClaws(Weapon::Type::Melee, "Vorpal Claws", 1, 6, 3, 3, -2, 1),
            m_spikedTail(Weapon::Type::Melee, "Spiked Tail", 3, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, MONSTERS_OF_CHAOS, MONSTER, JABBERSLYTHE};
        m_weapons = {&m_slytheyTongue, &m_vorpalClaws, &m_spikedTail};
        m_battleFieldRole = Role::Behemoth;

        setGreatfray(fray);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_slytheyTongue);
        model->addMeleeWeapon(&m_vorpalClaws);
        model->addMeleeWeapon(&m_spikedTail);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Jabberslythe::Create(const ParameterList &parameters) {
        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        return new Jabberslythe(fray);
    }

    void Jabberslythe::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Jabberslythe", factoryMethod);
        }
    }

    int Jabberslythe::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    Wounds Jabberslythe::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto totalWounds = Unit::computeReturnedDamage(weapon, saveRoll);

        // Spurting Bile Blood
        if (Dice::RollD6() >= 4) {
            totalWounds.mortal++;
        }

        return totalWounds;
    }

    void Jabberslythe::onStartHero(PlayerId player) {
        BeastsOfChaosBase::onStartHero(player);

        // Aura of Madness
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0);
            for (auto unit : units) {
                if (unit->remainingModels() > 0) {
                    if (Dice::RollD6() == 6) {
                        const Duration duration = {GamePhase::Hero, m_battleRound + 1, owningPlayer()};
                        unit->buffMovement(MovementRule::Can_Move, false, duration);
                        unit->buffMovement(MovementRule::Can_PileIn, false, duration);
                        unit->buffMovement(MovementRule::Can_Charge, false, duration);
                        unit->buffMovement(MovementRule::Can_Retreat, false, duration);
                        unit->buffAbility(Ability::Can_Attack, 0, duration);
                        unit->buffAbility(Ability::Cast_Spells, 0, duration);
                        unit->buffAbility(Ability::Unbind_Spells, 0, duration);
                    }
                }
            }
        }
    }

} // namespace BeastsOfChaos
