/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "BonesplitterzLore.h"
#include "Board.h"

namespace Bonesplitterz {

    class SquigglyCurse : public Spell {
    public:
        SquigglyCurse(Unit* caster) :
                Spell(caster, "Squiggly Curse", 6, 3) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
            m_targetKeywords = {HERO};
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            Wounds wounds = {0, Dice::RollD3(), Wounds::Source::Spell};
            if (unmodifiedCastingValue.isDouble()) {
                wounds.mortal = Dice::RollD6();
            }
            auto numSlain = target->applyDamage(wounds, m_caster);
            if (numSlain) {
                m_caster->buffModifier(Casting_Roll, 1, {Phase::Hero, DurationRestOfGame, m_caster->owningPlayer()});
            }
            return Spell::Result::Success;
        }
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    class BreathOfGorkamorka : public Spell {
    public:
        BreathOfGorkamorka(Unit* caster) :
                Spell(caster, "Breath of Gorkamorka", 6, 24) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
            m_targetKeywords = {BONESPLITTERZ};
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            if (unmodifiedCastingValue.isDouble()) {
                target->buffMovement(Triple_Movement, true, defaultDuration());
            }
            else {
                target->buffMovement(Double_Movement, true, defaultDuration());
            }
            target->buffMovement(Can_Fly, true, defaultDuration());

            return Spell::Result::Success;
        }
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    class BrutalBeastSpirits : public Spell {
    public:
        BrutalBeastSpirits(Unit* caster) :
                Spell(caster, "BrutalBeastSpirits", 6, 24) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
            m_targetKeywords = {BONESPLITTERZ};
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            target->buffModifier(Run_Distance, 1, defaultDuration());
            target->buffModifier(Charge_Distance, 1, defaultDuration());
            target->buffModifier(To_Hit_Melee, 1, defaultDuration());
            target->buffModifier(To_Hit_Missile, 1, defaultDuration());
            if (unmodifiedCastingValue.isDouble()) {
                auto units = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), 24.0);
                for (auto unit : units) {
                    if (unit == target) continue;
                    if ((unit->remainingModels() > 0) && unit->hasKeyword(BONESPLITTERZ)) {
                        unit->buffModifier(Run_Distance, 1, defaultDuration());
                        unit->buffModifier(Charge_Distance, 1, defaultDuration());
                        unit->buffModifier(To_Hit_Melee, 1, defaultDuration());
                        unit->buffModifier(To_Hit_Missile, 1, defaultDuration());
                        break;
                    }
                }
            }
            return Spell::Result::Success;
        }
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    class BoneKrusha : public Spell {
    public:
        BoneKrusha(Unit* caster) :
                Spell(caster, "Bone Krusha", 6, 24) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            Wounds wounds = {0, 1, Wounds::Source::Spell};
            const auto dist = m_caster->distanceTo(target);
            if (dist < 6) {
                wounds.mortal = Dice::RollD6();
            }
            else if (dist < 12) {
                wounds.mortal = Dice::RollD3();
            }
            target->applyDamage(wounds, m_caster);
            return Spell::Result::Success;
        }
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    class GorkamorkasWarCry : public Spell {
    public:
        GorkamorkasWarCry(Unit* caster) :
                Spell(caster, "Gorkamorka's War Cry", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            target->buffAbility(Fights_Last, true, defaultDuration());

            return Spell::Result::Success;
        }
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch(which) {
            case Lore::Squiggly_Curse:
                return new SquigglyCurse(caster);
            case Lore::Breath_Of_Gorkamorka:
                return new BreathOfGorkamorka(caster);
            case Lore::Brutal_Beast_Spirits:
                return new BrutalBeastSpirits(caster);
            case Lore::Bone_Krusha:
                return new BoneKrusha(caster);
            case Lore::Kunnin_Beast_Spirits:
                return new BuffModifierSpell(caster, "Kunnin' Beast Spirits", 6, 24,
                                             {{To_Save_Melee, 1}, {To_Save_Missile, 1}},
                                             Abilities::Target::SelfAndFriendly, std::vector<Keyword>{BONESPLITTERZ});
            case Lore::Gorkamorkas_War_Cry:
                return new GorkamorkasWarCry(caster);
            default:
                break;
        }
        return nullptr;
    }

} // namespace Bonesplitterz
