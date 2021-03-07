/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Board.h>
#include "IronjawzLore.h"

namespace Ironjawz {

    class BrainBursta : public Spell {
    public:
        explicit BrainBursta(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    BrainBursta::BrainBursta(Unit *caster) :
            Spell(caster, "Brain-bursta", 5, 16) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result BrainBursta::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr)
            return Spell::Result::Failed;

        Wounds wounds = { 0, Dice::RollD3(),Wounds::Source::Spell };
        if (Dice::Roll2D6() > target->bravery()) {
            wounds = { 0, Dice::RollD6(),Wounds::Source::Spell };
        }
        target->applyDamage(wounds, m_caster);

        return Spell::Result::Success;
    }

    class MightyEadbutt : public Spell {
    public:
        MightyEadbutt(Unit* caster) :
            Spell(caster, "Mighty 'Eadbutt", 5, 16) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
            m_targetKeywords = {HERO, WIZARD};
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            Wounds wounds = { 0, Dice::RollD3(),Wounds::Source::Spell };
            if (target->hasKeyword(WIZARD)) {
                wounds = { 0, Dice::RollD6(),Wounds::Source::Spell };
            }
            target->applyDamage(wounds, m_caster);

            return Spell::Result::Success;
        }
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    class WrathOfGork : public Spell {
    public:
        WrathOfGork(Unit* caster) :
                Spell(caster, "Wrath of Gork", 8, 16) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            int count = 0;
            auto units = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), 16.0);
            for (auto unit : units) {
                if ((unit->remainingModels() >= 2) && unit->hasKeyword(IRONJAWZ)) {
                    count++;
                }
            }
            Dice::RollResult rolls;
            Dice::RollD6(count*2, rolls);
            Wounds wounds = { 0, rolls.rollsGE(2),Wounds::Source::Spell };
            target->applyDamage(wounds, m_caster);

            return Spell::Result::Success;
        }
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Brain_Bursta:
                return new BrainBursta(caster);
            case Lore::Mighty_Eadbutt:
                return new MightyEadbutt(caster);
            case Lore::Da_Blazin_Eyes:
                return new LineOfEffectSpell(caster, "Da Blazin' Eyes", 6, RAND_4D6, 1, 4);
            case Lore::Da_Great_Big_Green_Hand_Of_Gork:
                return nullptr;
            case Lore::Bash_Em_Ladz:
                return nullptr;
            case Lore::Wrath_Of_Gork:
                return new WrathOfGork(caster);
            default:
                break;
        }
        return nullptr;
    }

} // namespace Ironjawz
