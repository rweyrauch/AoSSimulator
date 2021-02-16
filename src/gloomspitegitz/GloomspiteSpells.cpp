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

        Result cast(Unit *target, int round) override;

        Result cast(double x, double y, int round) override { return Result::Failed; }
    };

    GreatGreenSpite::GreatGreenSpite(Unit *caster) :
            Spell(caster, "The Great Green Spite", 7, 18) {
        m_allowedTargets = Target::Enemy;
        m_effect = EffectType::Damage;
        m_targetKeywords.push_back(GLOOMSPITE_GITZ);
    }

    Spell::Result GreatGreenSpite::cast(Unit *target, int round) {
        if (target == nullptr) {
            return Result::Failed;
        }

        // Distance to target
        const double distance = m_caster->distanceTo(target);
        if (distance > m_range) {
            return Result::Failed;
        }

        auto unit = Board::Instance()->getNearestUnit(m_caster, GetEnemyId(m_caster->owningPlayer()));

        // Check for visibility to enemy unit
        if ((unit == nullptr) || !Board::Instance()->isVisible(m_caster, unit)) {
            return Result::Failed;
        }

        Spell::Result result = Result::Failed;

        int damage = 1;
        if (target->remainingModels() >= 20)
            damage = RAND_D6;
        else if (target->remainingModels() >= 10)
            damage = RAND_D3;

        int mortalWounds = 0;
        const int castingRoll = m_caster->rollCasting();
        if (castingRoll >= m_castingValue) {
            bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
            if (!unbound) {
                mortalWounds = Dice::RollSpecial(damage);
                target->applyDamage({0, mortalWounds, Wounds::Source::Spell}, m_caster);
                SimLog(Verbosity::Narrative,
                       "%s spell %s with casting roll of %d (%d) inflicts %d mortal wounds into %s.\n",
                       m_caster->name().c_str(), name().c_str(), castingRoll, m_castingValue, mortalWounds,
                       target->name().c_str());
                result = Result::Success;
            } else {
                result = Result::Unbound;
            }
        }

        return result;
    }

    class SquigLure : public Spell {
    public:
        explicit SquigLure(Unit *caster);

        Result cast(Unit *target, int round) override;

        Result cast(double x, double y, int round) override { return Result::Failed; }
    };

    SquigLure::SquigLure(Unit *caster) :
        Spell(caster, "Squig Lure", 5, 18) {
        m_allowedTargets = Target::Friendly;
        m_effect = EffectType::Buff;
        m_targetKeywords.push_back(SQUIG);
    }

    Spell::Result SquigLure::cast(Unit *target, int round) {

        // Distance to target
        const double distance = m_caster->distanceTo(target);
        if (distance > m_range) {
            return Result::Failed;
        }

        // Check for visibility to target
        if (!Board::Instance()->isVisible(m_caster, target)) {
            return Result::Failed;
        }

        Spell::Result result = Result::Failed;

        const int castingRoll = m_caster->rollCasting();
        if (castingRoll >= m_castingValue) {
            bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
            if (!unbound) {
                auto squigs = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), m_range);
                auto numUnits = Dice::RollD3();
                int numAffected = 0;
                for (auto squig : squigs) {
                    squig->buffMovement(Run_And_Charge, true, {Phase::Hero, round + 1, m_caster->owningPlayer()});

                    numAffected++;
                    if (numAffected > numUnits) break;
                }
                result = Result::Success;
            } else {
                SimLog(Verbosity::Narrative, "%s spell %s was unbound.\n", m_caster->name().c_str(), name().c_str());
                result = Result::Unbound;
            }
        } else {
            SimLog(Verbosity::Narrative, "%s spell %s failed with roll %d needing %d.\n", m_caster->name().c_str(),
                   name().c_str(),
                   castingRoll, m_castingValue);
        }

        return result;
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