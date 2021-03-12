/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Lore.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {

    class PavaneOfSlaanesh : public Spell {
    public:
       explicit PavaneOfSlaanesh(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
   };

    PavaneOfSlaanesh::PavaneOfSlaanesh(Unit *caster) :
        Spell(caster, "Pavane of Slaanesh", 6, 6) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
        m_targetKeywords.push_back(HERO);
    }

    Spell::Result PavaneOfSlaanesh::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        Dice::RollResult roll;
        Dice::RollD6(target->move(), roll);
        target->applyDamage({ 0, roll.rollsGE(5),Wounds::Source::Spell }, m_caster);

        return Spell::Result::Success;
    }

    class HystericalFrenzy : public Spell {
    public:
        explicit HystericalFrenzy(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    HystericalFrenzy::HystericalFrenzy(Unit *caster) :
            Spell(caster, "Hysterical Frenzy", 6, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result HystericalFrenzy::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        Dice::RollResult roll;
        Dice::RollD6(target->remainingModels(), roll);
        target->applyDamage({ 0, roll.rollsGE(6),Wounds::Source::Spell }, m_caster);

        return Spell::Result::Success;
    }

    class SoulsliceShards : public Spell {
    public:
        explicit SoulsliceShards(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    SoulsliceShards::SoulsliceShards(Unit *caster) :
            Spell(caster, "Soulslice Shards", 5, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result SoulsliceShards::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        auto roll = Dice::Roll2D6();
        if (roll > target->bravery()) {
            target->applyDamage({0, roll - target->bravery(), Wounds::Source::Spell}, m_caster);
        }
        return Spell::Result::Success;
    }

    class Phantasmagoria : public Spell {
    public:
        explicit Phantasmagoria(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    Phantasmagoria::Phantasmagoria(Unit *caster) :
            Spell(caster, "Phantasmagoria", 7, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Debuff;
    }

    Spell::Result Phantasmagoria::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        Dice::RollResult roll;
        Dice::RollD6(6, roll);

        target->buffModifier(Attribute::Bravery, roll.rollsGE(5), defaultDuration());

        return Spell::Result::Success;
    }

    class PathsOfTheDarkPrince : public Spell {
    public:
        explicit PathsOfTheDarkPrince(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    PathsOfTheDarkPrince::PathsOfTheDarkPrince(Unit *caster) :
        Spell(caster, "Paths of the Dark Prince", 7, 0) {
        m_allowedTargets = Abilities::Target::Self;
        m_effect = Abilities::EffectType::Buff;
    }

    Spell::Result PathsOfTheDarkPrince::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        target->buffMovement(MovementRule::Can_Fly, true, defaultDuration());

        return Spell::Result::Success;
    }

    class JudgementOfExcess : public Spell {
    public:
        explicit JudgementOfExcess(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    JudgementOfExcess::JudgementOfExcess(Unit *caster) :
            Spell(caster, "Judgement of Excess", 5, 12) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result JudgementOfExcess::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        auto numWounds = (target->remainingModels() + 5)/5;
        target->applyDamage({0, numWounds, Wounds::Source::Spell}, m_caster);

        return Spell::Result::Success;
    }

    class BattleRapture : public Spell {
    public:
        explicit BattleRapture(Unit *caster);

    protected:

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    BattleRapture::BattleRapture(Unit *caster) :
            Spell(caster, "Battle Rapture", 5, 18) {
        m_allowedTargets = Abilities::Target::Friendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords.push_back(MORTAL);
        m_targetKeywords.push_back(SLAANESH);
    }

    Spell::Result BattleRapture::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        auto units = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), m_range);
        auto numUnits = (castingValue >= 10) ? 3 : 1;
        int numAffected = 0;
        for (auto unit : units) {
            if (unit->hasKeyword(MORTAL) && unit->hasKeyword(SLAANESH)) {
                unit->buffModifier(Attribute::Bravery, 10, defaultDuration());
                numAffected++;
            }
            if (numAffected > numUnits) break;
        }
        return Result::Success;
    }

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Lash_Of_Slaanesh:
                return new LineOfEffectSpell(caster, "Lash of Slaanesh", 5, 12, 1, 4);
            case Lore::Pavane_Of_Slaanesh:
                return new PavaneOfSlaanesh(caster);
            case Lore::Hysterical_Frenzy:
                return new HystericalFrenzy(caster);
            case Lore::Soulslice_Shards:
                return new SoulsliceShards(caster);
            case Lore::Phantasmagoria:
                return new Phantasmagoria(caster);
            case Lore::Born_Of_Damnation:
                return new HealSpell(caster, "Born of Damnation", 4, 6, RAND_D3, -1, -1, {HEDONITE, HERO});
            case Lore::Paths_Of_The_Dark_Prince:
                return new PathsOfTheDarkPrince(caster);
            case Lore::Progeny_Of_Damnation:
                return new HealSpell(caster, "Progeny of Damnation", 5, 6, RAND_D3, 10, RAND_D6, {DAEMON, HEDONITE, HERO});
            case Lore::Slothful_Stupor:
                // TODO: Slothful_Stupor
                return nullptr;
            case Lore::Battle_Rapture:
                return new BattleRapture(caster);
            case Lore::Judgement_Of_Excess:
                return new JudgementOfExcess(caster);
            case Lore::Dark_Delusions:
                // TODO: Dark_Delusions
                return nullptr;
            default:
                return nullptr;
        }
    }

    class Acquiescence : public Spell {
    public:
        explicit Acquiescence(Unit* caster) :
                Spell(caster, "Acquiescence", 5, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            target->buffReroll(Attribute::Target_To_Hit_Melee, Rerolls::Ones, defaultDuration());
            target->buffReroll(Attribute::Target_To_Hit_Missile, Rerolls::Ones, defaultDuration());
            return Result::Success;
        }
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    Spell *CreateAcquiescence(Unit *caster) {
        return new Acquiescence(caster);
    }

} //namespace Slaanesh
