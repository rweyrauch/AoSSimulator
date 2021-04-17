/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/FjulGrimnir.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers {

    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 100;

    bool FjulGrimnir::s_registered = false;

    FjulGrimnir::FjulGrimnir() :
            Fyreslayer("Fjul-Grimnir", 4, g_wounds, 8, 4, false),
            m_grandAxe(Weapon::Type::Melee, "Latchkey Grandaxe", 3, 3, 3, 3, -1, 3) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, AURIC_RUNEFATHER, FJUL_GRIMNIR};
        m_weapons = {&m_grandAxe};
        m_battleFieldRole = Role::Leader;
    }

    void FjulGrimnir::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_grandAxe);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *FjulGrimnir::Create(const ParameterList &parameters) {
        auto unit = new FjulGrimnir();

        unit->setLodge(Lodge::Vostarg);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void FjulGrimnir::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FjulGrimnir::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    FjulGrimnir::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Fjul-Grimnir", factoryMethod);
        }
    }

    int FjulGrimnir::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void FjulGrimnir::onStartHero(PlayerId player) {
        Fyreslayer::onStartHero(player);

        if (owningPlayer() == player) {
            // Stare Down
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0)) {
                unit->buffModifier(Attribute::Bravery, -Dice::RollD3(),
                                   {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
            }
        }
    }

    void FjulGrimnir::onEndCombat(PlayerId player) {
        Unit::onEndCombat(player);

        // Weapon-breaker
        auto unit = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
        if (unit) {
            if (Dice::RollD6() == 6) {
                // TODO: buff only affects a single weapon
                unit->buffModifier(Attribute::To_Hit_Melee, -1,
                                   {GamePhase::Battleshock, std::numeric_limits<int>::max(), owningPlayer()});
            }
        }
    }

    Wounds FjulGrimnir::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        Wounds totalWounds = wounds;
        // Grimnir's Blessing
        auto chosen = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), CHOSEN_AXES, 3.0);
        if (chosen != nullptr) {
            totalWounds = ignoreWounds(totalWounds, 5);
        }
        return Unit::applyWoundSave(totalWounds, attackingUnit);
    }

} // namespace Fyreslayers