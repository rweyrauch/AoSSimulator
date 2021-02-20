/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include "MawtribesLore.h"
#include "Unit.h"
#include "Spell.h"
#include "Prayer.h"

namespace OgorMawtribes {

    class FleshcraveCurse : public Spell {
    public:
        FleshcraveCurse(Unit *caster) :
                Spell(caster, "Fleshcrave Curse", 6, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }
    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            target->applyDamage({0, Dice::RollD6(), Wounds::Source::Spell}, m_caster);
            target->buffModifier(Attacks_Melee, 1, defaultDuration());
            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    class Ribcracker : public Spell {
    public:
        Ribcracker(Unit *caster) :
                Spell(caster, "Ribcracker", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }
    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(To_Save_Melee, -1, defaultDuration());
            target->buffModifier(To_Save_Missile, -1, defaultDuration());
            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    class GreasyDeluge : public Spell {
    public:
        GreasyDeluge(Unit *caster) :
                Spell(caster, "Greasy Deluge", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }
    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(To_Hit_Melee, -1, defaultDuration());
            target->buffModifier(To_Hit_Missile, -1, defaultDuration());
            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    class FieryWhirlwind : public Spell {
    public:
        FieryWhirlwind(Unit *caster) :
                Spell(caster, "Fiery Whirlwind", 6, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }
    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            int numRolls = target->remainingModels();
            if (numRolls == 1) numRolls = 3;
            Dice::RollResult rolls;
            Dice::RollD6(numRolls, rolls);
            target->applyDamage({0, rolls.rollsGE(4), Wounds::Source::Spell}, m_caster);
            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Fleshcrave_Curse:
                return new FleshcraveCurse(caster);
            case Lore::Blood_Feast:
                return new BuffModifierSpell(caster, "Blood Feast", 7, 18, Attacks_Melee, 1, Abilities::Target::SelfAndFriendly, {OGOR});
            case Lore::Ribcracker:
                return new Ribcracker(caster);
            case Lore::Blubbergrub_Stench:
                // TODO: Blubbergrub_Stench
                return nullptr;
            case Lore::Molten_Entrails:
                // TODO: Molten_Entrails
                return nullptr;
            case Lore::Greasy_Deluge:
                return new GreasyDeluge(caster);
            case Lore::Fiery_Whirlwind:
                return new FieryWhirlwind(caster);
            case Lore::Billowing_Ash:
                // TODO: Billowing_Ash
                return nullptr;
            case Lore::Tongues_Of_Flame:
                // TODO: Tongues_Of_Flame
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }


    class PulverisingHailstorm : public ::Prayer {
    public:
        PulverisingHailstorm(Unit *priest) :
                Prayer(priest, "Pulverising Hailstorm", 4, 18, 0) {
            m_allowedTargets = Abilities::Target::Point;
            m_effect = Abilities::EffectType::AreaOfEffectDamage;
        }
        bool apply(int prayingRoll, Unit *target) override {
            if (target == nullptr) {
                return false;
            }
            return apply(prayingRoll, target->x(), target->y());
        }
        bool apply(int prayingRoll, double x, double y) override {
            auto units = Board::Instance()->getUnitsWithin({x, y, 0.0}, GetEnemyId(m_priest->owningPlayer()), 3.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 3) {
                    unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Prayer}, m_priest);
                }
            }
            return true;
        }
    };

    class KeeningGale : public ::Prayer {
    public:
        KeeningGale(Unit *priest) :
                Prayer(priest, "Keening Gale", 4, 18, 0) {
            m_allowedTargets = Abilities::Target::Friendly;
            m_effect = Abilities::EffectType::Buff;
            m_targetKeywords = {MONSTER, MOURNFANG_PACK};
        }
        bool apply(int prayingRoll, Unit *target) override {
            if (target == nullptr) {
                return false;
            }

            if (target->hasKeyword(MONSTER) || target->hasKeyword(MOURNFANG_PACK)) {
                target->buffModifier(Move_Distance, 3, {Phase::Hero, m_round + 1, m_priest->owningPlayer()});
            }
            return true;
        }
        bool apply(int prayingRoll, double x, double y) override { return false; }
    };

    class CallOfTheBlizzard : public ::Prayer {
    public:
        CallOfTheBlizzard(Unit *priest) :
                Prayer(priest, "Call of the Blizzard", 4, 18, 0) {
            m_allowedTargets = Abilities::Target::Friendly;
            m_effect = Abilities::EffectType::Heal;
            m_targetKeywords = {ICEFALL_YHETESS};
        }
        bool apply(int prayingRoll, Unit *target) override {
            if (target == nullptr) {
                return false;
            }

            if (target->hasKeyword(ICEFALL_YHETESS)) {
                target->returnModels(1);
            }
            return true;
        }
        bool apply(int prayingRoll, double x, double y) override { return false; }
    };

    ::Prayer* CreatePrayer(Prayer which, Unit* priest) {
        switch (which) {
            case Prayer::Pulverising_Hailstorm:
                return new PulverisingHailstorm(priest);
            case Prayer::Keening_Gale:
                return new KeeningGale(priest);
            case Prayer::Call_Of_The_Blizzard:
                return new CallOfTheBlizzard(priest);
            default:
                break;
        }
        return nullptr;
    }

} // namespace OgorMawtribes
