/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/GloomspiteSpells.h>
#include <Unit.h>
#include <Board.h>
#include <magic_enum.hpp>

namespace GloomspiteGitz {

    std::string ToString(Lore which) {
        return std::string(magic_enum::enum_name<Lore>(which));
    }

    bool FromString(const std::string &enumString, Lore &outLore) {
        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value())
            outLore = lore.value();
        return lore.has_value();
    }

    DamageSpell *CreateVindictiveGlare(Unit *caster) {
        return new DamageSpell(caster, "Vindictive Glare", 5, 12, RAND_D3);
    }

    class GreatGreenSpite : public Spell {
    public:
        explicit GreatGreenSpite(Unit *caster);

    protected:

        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    GreatGreenSpite::GreatGreenSpite(Unit *caster) :
            Spell(caster, "The Great Green Spite", 7, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
        m_targetKeywords.push_back(GLOOMSPITE_GITZ);
    }

    Spell::Result GreatGreenSpite::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
        if (target == nullptr) {
            return Result::Failed;
        }

        auto unit = Board::Instance()->getNearestUnit(m_caster, GetEnemyId(m_caster->owningPlayer()));

        // Check for visibility to enemy unit
        if ((unit == nullptr) || !Board::Instance()->isVisible(m_caster, unit)) {
            return Result::Failed;
        }

        int damage = 1;
        if (target->remainingModels() >= 20)
            damage = RAND_D6;
        else if (target->remainingModels() >= 10)
            damage = RAND_D3;

        int mortalWounds = 0;
        mortalWounds = Dice::RollSpecial(damage);
        target->applyDamage({0, mortalWounds, Wounds::Source::Spell}, m_caster);
        PLOG_INFO.printf("%s spell %s with casting roll of %d (%d) inflicts %d mortal wounds into %s.\n",
               m_caster->name().c_str(), name().c_str(), castingValue, m_castingValue, mortalWounds,
               target->name().c_str());

        return Result::Success;
    }

    class SquigLure : public Spell {
    public:
        explicit SquigLure(Unit *caster);

    protected:

        Result apply(int castingValue, int unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    SquigLure::SquigLure(Unit *caster) :
        Spell(caster, "Squig Lure", 5, 18) {
        m_allowedTargets = Abilities::Target::Friendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords.push_back(SQUIG);
    }

    Spell::Result SquigLure::apply(int castingValue, int unmodifiedCastingValue, Unit *target) {
        auto squigs = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), m_range);
        auto numUnits = Dice::RollD3();
        int numAffected = 0;
        for (auto squig : squigs) {
            squig->buffMovement(Run_And_Charge, true, defaultDuration());

            numAffected++;
            if (numAffected > numUnits) break;
        }
        return Result::Success;
    }


    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::None:
                return nullptr;
            case Lore::Vindictive_Glare:
                return CreateVindictiveGlare(caster);
            case Lore::Itchy_Nuisance:
                return nullptr;
            case Lore::The_Great_Green_Spite:
                return new GreatGreenSpite(caster);
            case Lore::The_Hand_Of_Gork:
                return nullptr;
            case Lore::Squig_Lure:
                return new SquigLure(caster);
            case Lore::Call_Da_Moon:
                return nullptr;
            case Lore::Deadly_Webbing:
                return nullptr;
            case Lore::Venomous_Spiderlings:
                return nullptr;
            case Lore::Scuttling_Terrors:
                return nullptr;
            case Lore::Sneaky_Distraction:
                return nullptr;
            case Lore::Curse_Of_Da_Spider_God:
                return nullptr;
            case Lore::Gift_Of_Da_Spider_God:
                return nullptr;
        }
        return nullptr;
    }

} // namespace GloomspiteGitz