/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Lore.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {

    class PavaneOfSlaanesh : public Spell {
    public:
       explicit PavaneOfSlaanesh(Unit* caster);

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
   };

    PavaneOfSlaanesh::PavaneOfSlaanesh(Unit *caster) :
        Spell(caster, "Pavane of Slaanesh", 6, 6) {
        m_allowedTargets = Spell::Target::Enemy;
        m_effect = Spell::EffectType::Damage;
        m_targetKeywords.push_back(HERO);
    }

    Spell::Result PavaneOfSlaanesh::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
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
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    HystericalFrenzy::HystericalFrenzy(Unit *caster) :
            Spell(caster, "Hysterical Frenzy", 6, 18) {
        m_allowedTargets = Spell::Target::Enemy;
        m_effect = Spell::EffectType::Damage;
    }

    Spell::Result HystericalFrenzy::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
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
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    SoulsliceShards::SoulsliceShards(Unit *caster) :
            Spell(caster, "Soulslice Shards", 5, 18) {
        m_allowedTargets = Spell::Target::Enemy;
        m_effect = Spell::EffectType::Damage;
    }

    Spell::Result SoulsliceShards::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
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
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    Phantasmagoria::Phantasmagoria(Unit *caster) :
            Spell(caster, "Phantasmagoria", 7, 18) {
        m_allowedTargets = Spell::Target::Enemy;
        m_effect = Spell::EffectType::Debuff;
    }

    Spell::Result Phantasmagoria::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        auto Dice::RollResult roll;
        Dice::RollD6(6, roll);

        target->buffModifier(Bravery, roll.rollGE(5), defaultDuration());

        return Spell::Result::Success;
    }

    class PathsOfTheDarkPrince : public Spell {
    public:
        explicit PathsOfTheDarkPrince(Unit* caster);

    protected:
        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    PathsOfTheDarkPrince::PathsOfTheDarkPrince(Unit *caster) :
        Spell(caster, "Paths of the Dark Prince", 7, 0) {
        m_allowedTargets = Spell::Target::Self;
        m_effect = Spell::EffectType::Buff;
    }

    Spell::Result PathsOfTheDarkPrince::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        target->buffMovement(Fly, true, defaultDuration());

        return Spell::Result::Success;
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
                return nullptr;
            case Lore::Battle_Rapture:
                return nullptr;
            case Lore::Judgement_Of_Excess:
            case Lore::Dark_Delusions:
                return nullptr;
            default:
                return nullptr;
        }
    }

} //namespace Slaanesh
