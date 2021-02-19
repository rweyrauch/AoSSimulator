/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <skaven/SkavenSpells.h>
#include <Board.h>
#include <Unit.h>

namespace Skaven {

    class Scorch : public Spell {
    public:
        explicit Scorch(Unit *caster) :
                Spell(caster, "Scorch", 5, 13) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            Dice::RollResult rolls;
            Dice::RollD6(castingRoll, rolls);
            target->applyDamage({0, rolls.rollsGE(6), Wounds::Source::Spell}, m_caster);
            return Result::Success;
        }

        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    class Splinter : public Spell {
    public:
        explicit Splinter(Unit *caster) :
                Spell(caster, "Splinter", 6, 6) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            if (Dice::RollD6() > target->wounds()) {
                target->slay(1);
            }
            return Result::Success;
        }

        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    class Plague : public Spell {
    public:
        explicit Plague(Unit *caster) :
                Spell(caster, "Plague", 7, 13) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            Dice::RollResult rolls;
            Dice::RollD6(target->remainingModels(), rolls);
            target->applyDamage({0, rolls.rollsGE(5), Wounds::Source::Spell}, m_caster);
            return Result::Success;
        }

        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    class Warpgale : public Spell {
    public:
        explicit Warpgale(Unit *caster) :
                Spell(caster, "Warpgale", 8, 26) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            target->buffMovement(Halve_Charge_Roll, true, defaultDuration());
            target->buffMovement(Halve_Run_Roll, true, defaultDuration());
            if (target->fly()) {
                target->buffMovement(Fly, false, defaultDuration());
            }
            return Result::Success;
        }

        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Spell::Result::Failed; }
    };

    class ChainWarpLightning : public Spell {
    public:
        explicit ChainWarpLightning(Unit *caster) :
                Spell(caster, "Chain Warp Lightning", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::AreaOfEffectDamage;
        }

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) override {
            auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), m_range);
            int numUnitAffected = Dice::RollD6();
            numUnitAffected = std::min(numUnitAffected, (int)units.size());
            for (auto i = 0; i < numUnitAffected; i++) {
                units[i]->applyDamage({0, 1, Wounds::Source::Spell}, m_caster);
            }
            return Result::Success;
        }

        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override {
            return apply(castingRoll, unmodifiedCastingRoll, nullptr);
        }
    };

    DamageSpell *CreateWarpLightning(Unit *caster) {
        // TODO: Implement the 'warp-power accumulator' option.
        return new DamageSpell(caster, "Warp Lightning", 5, 13, RAND_D3);
    }

    AreaOfEffectSpell *CreateWarpLightningStorm(Unit *caster) {
        // TODO: Implement the 'warp-power accumulator' option.
        // TODO: Storm is limited to D3 units (really D3 casts of WarpLightning)
        return new AreaOfEffectSpell(caster, "Warp Lightning Storm", 7, 0, 13, RAND_D3, 0);
    }

    Spell* CreateLore(Lore which, Unit* caster) {
        switch (which) {
            case Lore::Scorch:
                return new Scorch(caster);
            case Lore::Splinter:
                return new Splinter(caster);
            case Lore::Skitterleap:
                // TODO: Skitterleap
                return nullptr;
            case Lore::Plague:
                return new Plague(caster);
            case Lore::Death_Frenzy:
                // TODO: Death_Frenzy
                return nullptr;
            case Lore::Warpgale:
                return new Warpgale(caster);
            case Lore::More_More_More_Warp_Power:
                // TODO: More_More_More_Warp_Power
                return nullptr;
            case Lore::Chain_Warp_Lightning:
                return new ChainWarpLightning(caster);
            case Lore::Warp_Lighning_Shield:
                // TODO: Warp_Lighning_Shield
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }
}

