/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "TzeentchSpells.h"
#include "Unit.h"
#include "Roster.h"

namespace Tzeentch {

    class BoltOfChange : public Spell {
    public:
        explicit BoltOfChange(Unit* caster);

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override;
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    BoltOfChange::BoltOfChange(Unit *caster) :
            Spell(caster, "Bolt of Change", 7, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result BoltOfChange::apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        auto numSlain = target->applyDamage({0, Dice::RollSpecial(RAND_D3), Wounds::Source::Spell}, m_caster);
        if (numSlain > 0) {
            // Add a Chaos Spawn to this roster
            auto factory = UnitFactory::LookupUnit("Chaos Spawn");
            if (factory) {
                if (m_caster->getRoster()) {
                    auto unit = std::shared_ptr<Unit>(UnitFactory::Create("Chaos Spawn", factory->m_parameters));
                    unit->deploy(m_caster->position(), m_caster->orientation());
                    m_caster->getRoster()->addUnit(unit);
                }
            }
        }
        return Spell::Result::Success;
    }

    class InfusionArcanum : public Spell {
    public:
        explicit InfusionArcanum(Unit* caster) :
                Spell(caster, "Infusion Arcanum", 5, 0) {
            m_allowedTargets = Abilities::Target::Self;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override {
            m_caster->buffModifier(To_Hit_Melee, 1, defaultDuration());
            m_caster->buffModifier(To_Hit_Missile, 1, defaultDuration());
            m_caster->buffModifier(To_Wound_Melee, 1, defaultDuration());
            m_caster->buffModifier(To_Wound_Missile, 1, defaultDuration());

            return Spell::Result::Success;
        }
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    class TreasonOfTzeentch : public Spell {
    public:
        explicit TreasonOfTzeentch(Unit* caster) :
                Spell(caster, "Treason of Tzeentch", 5, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }

            Dice::RollResult rolls;
            Dice::RollD6(target->remainingModels(), rolls);
            if (rolls.rollsGE(6) > 0) {
                int numSlain = target->applyDamage({0, rolls.rollsGE(6), Wounds::Source::Spell}, m_caster);
                if (numSlain > 0) {
                    target->buffModifier(To_Hit_Missile, -1, defaultDuration());
                    target->buffModifier(To_Hit_Melee, -1, defaultDuration());
                }
            }
            return Spell::Result::Success;
        }
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    class UncheckedMutation : public Spell {
    public:
        explicit UncheckedMutation(Unit* caster) :
                Spell(caster, "Unchecked Mutation", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }

            int numSlain = target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            if (numSlain > 0) {
                if (Dice::RollD6() >= 3) {
                    target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
                }
            }
            return Spell::Result::Success;
        }
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    class TzeentchsFirestorm : public Spell {
    public:
        explicit TzeentchsFirestorm(Unit* caster) :
                Spell(caster, "Tzeentch's Firestorm", 9, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }

            Dice::RollResult rolls;
            Dice::RollD6(9, rolls);
            Wounds wounds{0, 0, Wounds::Source::Spell};
            for (auto i = 0; i < rolls.rollsGE(6); i++) {
                wounds.mortal += Dice::RollD3();
            }
            target->applyDamage(wounds, m_caster);
            return Spell::Result::Success;
        }
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    Spell* CreateBoltOfChange(Unit* caster) {
        return new BoltOfChange(caster);
    }

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Bolt_Of_Tzeentch:
                return new DamageSpell(caster, "Bolt of Tzeentch", 7, 18, RAND_D6);
            case Lore::Arcane_Suggestion:
                // TODO: Arcane_Suggestion
                return nullptr;
            case Lore::Glimpse_The_Future:
                // TODO: Glimpse_The_Future
                return nullptr;
            case Lore::Shield_Of_Faith:
                // TODO: Shield_Of_Faith
                return nullptr;
            case Lore::Infusion_Arcanum:
                return new InfusionArcanum(caster);
            case Lore::Treacherous_Bond:
                // TODO: Treacherous_Bond
                return nullptr;
            case Lore::Treason_Of_Tzeentch:
                return new TreasonOfTzeentch(caster);
            case Lore::Arcane_Transformation:
                // TODO: Arcane_Transformation
                return nullptr;
            case Lore::Unchecked_Mutation:
                return new UncheckedMutation(caster);
            case Lore::Fold_Reality:
                // TODO: Fold_Reality
                return nullptr;
            case Lore::Tzeentchs_Firestorm:
                return new TzeentchsFirestorm(caster);
            default:
                break;
        }
        return nullptr;
    }

}
