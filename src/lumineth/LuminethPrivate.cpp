/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    PowerOfHysh::PowerOfHysh(Unit *caster) :
            Spell(caster, "Power of Hysh", 6, 3) {
        m_allowedTargets = Abilities::Target::Self;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords = {Sunmetal_Weapons};
    }

    Spell::Result
    PowerOfHysh::apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) {
        if (target == nullptr) {
            return Result::Failed;
        }
        target->buffAbility(Ability::Extra_Mortal_Wound_On_Hit_Roll, 5, defaultDuration());

        return Result::Success;
    }

    class SpeedOfHysh : public Spell {
    public:
        explicit SpeedOfHysh(Unit *caster) :
                Spell(caster, "Speed of Hysh", 5, 18) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            target->buffModifier(Attribute::Move_Distance, target->move(), defaultDuration());
            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class ProtectionOfHysh : public Spell {
    public:
        explicit ProtectionOfHysh(Unit *caster) :
                Spell(caster, "Protection of Hysh", 8, 9) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            target->buffAbility(Ability::Ignore_All_Wounds_On_Value, 5, defaultDuration());
            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class UnyieldingCalm : public Spell {
    public:
        explicit UnyieldingCalm(Unit *caster) :
                Spell(caster, "Unyielding Calm", 4, 18) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
            m_targetKeywords = {LUMINETH_REALM_LORDS};
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            target->buffAbility(Ability::Ignore_Battleshock, 1, defaultDuration());
            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class Entomb : public Spell {
    public:
        explicit Entomb(Unit *caster) :
                Spell(caster, "Entomb", 7, 8) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            auto roll = Dice::RollD6();
            if (roll > target->wounds()) {
                target->slay(1);
            } else if (roll == 6) {
                target->applyDamage({0, Dice::RollD6(), Wounds::Source::Spell}, m_caster);
            }
            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class VoiceOfTheMountain : public Spell {
    public:
        explicit VoiceOfTheMountain(Unit *caster) :
                Spell(caster, "Voice of the Mountain", 6, 1000) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            target->buffModifier(Attribute::Bravery, -1, defaultDuration()); // Until your next Hero phase
            target->buffModifier(Attribute::Bravery, -1,
                                 {Phase::Battleshock, m_round, m_caster->owningPlayer()}); // Until end of this turn

            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class AssaultOfStone : public Spell {
    public:
        explicit AssaultOfStone(Unit *caster) :
                Spell(caster, "Assault of Stone", 8, 24) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            Wounds wounds{0, 0, Wounds::Source::Spell};
            for (auto i = 0; i < castingRoll; i++) {
                const auto roll = Dice::RollD6();
                if ((roll < target->save()) && (roll > 2)) {
                    wounds.mortal++;
                }
            }
            if (wounds.mortal > 0) {
                target->applyDamage(wounds, m_caster);
            }
            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class OverwhelmingHeat : public Spell {
    public:
        explicit OverwhelmingHeat(Unit *caster) :
                Spell(caster, "Overwhelming Heat", 7, 24) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            target->buffModifier(Attribute::Move_Distance, -target->move() / 2, defaultDuration());
            if (Dice::RollD6() >= target->save()) {
                target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            }
            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Speed_Of_Hysh:
                return new SpeedOfHysh(caster);
            case Lore::Solar_Flare:
                // TODO: Solar_Flare
                return nullptr;
            case Lore::Lambent_Light:
                return new BuffRerollSpell(caster, "Lambent Light", 5, 18, Attribute::Target_To_Hit_Missile,
                                           Rerolls::Failed, Abilities::Target::Enemy);
            case Lore::Etheral_Blessing:
                // TODO: Etheral_Blessing
                return nullptr;
            case Lore::Total_Eclipse:
                // TODO: Total_Eclipse
                return nullptr;
            case Lore::Protection_Of_Hysh:
                return new ProtectionOfHysh(caster);
            case Lore::Unyielding_Calm:
                return new UnyieldingCalm(caster);
            case Lore::Crippling_Vertigo:
                // TODO: Crippling_Vertigo
                return nullptr;
            case Lore::Voice_Of_The_Mountains:
                return new VoiceOfTheMountain(caster);
            case Lore::Living_Fissue:
                return new LineOfEffectSpell(caster, "Living Fissure", 6, 9, RAND_D3, 2);
            case Lore::Entomb:
                return new Entomb(caster);
            case Lore::Assault_Of_Stone:
                return new AssaultOfStone(caster);
            case Lore::Overwhelming_Heat:
                return new OverwhelmingHeat(caster);
            default:
                break;
        }
        return nullptr;
    }

} // namespace LuminethRealmLords

