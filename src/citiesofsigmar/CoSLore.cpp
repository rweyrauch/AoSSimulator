/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "CoSLore.h"

#include "Spell.h"
#include "Unit.h"
#include "Roster.h"

namespace CitiesOfSigmar {

    class WingsOfFire : public Spell {
    public:
        explicit WingsOfFire(Unit *caster) :
                Spell(caster, "Wings of Fire", 6, INT32_MAX) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(Attribute::Run_Distance, 1, defaultDuration());
            target->buffModifier(Attribute::Charge_Distance, 1, defaultDuration());
            target->buffMovement(MovementRule::Can_Fly, true, defaultDuration());
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    class TwinTailedComet : public Spell {
    public:
        explicit TwinTailedComet(Unit *caster) :
                Spell(caster, "Twin-tailed Comet", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            target->applyDamage(
                    {0, (target->remainingModels() >= 10) ? Dice::RollD6() : Dice::RollD3(), Wounds::Source::Spell},
                    m_caster);
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    class IronoakSkin : public Spell {
    public:
        explicit IronoakSkin(Unit *caster) :
                Spell(caster, "Ironoak Skin", 6, 18) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(Attribute::Target_To_Wound_Missile, -1, defaultDuration());
            target->buffModifier(Attribute::Target_To_Wound_Melee, -1, defaultDuration());
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    class DescendingAshCloud : public Spell {
    public:
        explicit DescendingAshCloud(Unit *caster) :
                Spell(caster, "Descending Ash Cloud", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());
            target->buffModifier(Attribute::To_Hit_Missile, -1, defaultDuration());
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    class ChokingFumes : public Spell {
    public:
        explicit ChokingFumes(Unit *caster) :
                Spell(caster, "Choking Fumes", 6, 15) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            Dice::RollResult rolls;
            Dice::RollD6(target->remainingModels(), rolls);
            target->applyDamage({0, rolls.rollsGE(5), Wounds::Source::Spell}, m_caster);
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    class AddCommandPoint : public Spell {
    public:
        explicit AddCommandPoint(Unit *caster, const std::string &name, int castingValue, int number) :
                Spell(caster, name, castingValue, 0),
                m_number(number) {
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            target->getRoster()->addCommandPoints(m_number);
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }

        int m_number = 0;
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Wings_Of_Fire:
                return new WingsOfFire(caster);
            case Lore::Cindercloud:
                // TODO: Cindercloud
                return nullptr;
            case Lore::Twin_Tailed_Comet:
                return new TwinTailedComet(caster);
            case Lore::Lifesurge:
                return new HealSpell(caster, "Lifesurge", 6, 18, RAND_D6);
            case Lore::Cage_Of_Thorns:
                // TODO: Cage_Of_Thorns
                return nullptr;
            case Lore::Ironoak_Skin:
                return new IronoakSkin(caster);
            case Lore::Descending_Ash_Cloud:
                return new DescendingAshCloud(caster);
            case Lore::Eroding_Blast:
                // TODO: Eroding_Blast
                return nullptr;
            case Lore::Choking_Fumes:
                return new ChokingFumes(caster);
            case Lore::Amber_Tide:
                return new BuffMovementSpell(caster, "Amber Tide", 6, 18, MovementRule::Halve_Movement, true,
                                             Abilities::Target::Enemy);
            case Lore::Phoenix_Cry:
                return new BuffModifierSpell(caster, "Phoenix Cry", 5, 18, Attribute::Bravery, -1,
                                             Abilities::Target::Enemy);
            case Lore::Golden_Mist:
                return new HealSpell(caster, "Golden Mist", 6, 12, RAND_D3, -1, -1, {PHOENICIUM});
            case Lore::Sap_Strength:
                // TODO: Sap_Strength
                return nullptr;
            case Lore::Shadow_Daggers:
                return new DamageSpell(caster, "Shadow Daggers", 7, 9, RAND_D6);
            case Lore::Vitriolic_Spray:
                // TODO: Vitriolic_Spray
                return nullptr;
            case Lore::Roaming_Wildfire:
                // TODO: Roaming_Wildfire
                return nullptr;
            case Lore::Sear_Wounds:
                return new HealSpell(caster, "Sear Wounds", 6, 18, RAND_D6);
            case Lore::Elemental_Cyclone:
                // TODO: Elemental_Cyclone
                return nullptr;
            case Lore::Warding_Brand:
                // TODO: Warding_Brand
                return nullptr;
            case Lore::Crystal_Aegis:
                // TODO: Crystal_Aegis
                return nullptr;
            case Lore::Ignite_Weapons:
                // TODO: Ignite_Weapons
                return nullptr;
            case Lore::Aura_Of_Glory:
                // TODO: Aura_Of_Glory
                return nullptr;
            case Lore::Strike_Of_Eagles:
                // TODO: Strike_Of_Eagles
                return nullptr;
            case Lore::Celestial_Visions:
                return new AddCommandPoint(caster, "Celestial Visions", 7, 1);
            case Lore::The_Withering:
                return new BuffModifierSpell(caster, "The Withering", 7, 18,
                                             {{Attribute::Target_To_Wound_Missile, 1}, {Attribute::Target_To_Wound_Melee,   1}},
                                             Abilities::Target::Enemy);
            case Lore::Steed_Of_Shadows:
                // TODO: Steed_Of_Shadows
                break;
            case Lore::Pit_Of_Shades:
                // TODO: Pit_Of_Shades
                break;
            case Lore::Drain_Magic:
                // TODO: Drain_Magic
                break;
            case Lore::Shield_Of_Light:
                // TODO: Shield_Of_Light
                break;
            case Lore::Illuminate:
                return new BuffModifierSpell(caster, "Illuminate", 6, 12, Attribute::Target_To_Hit_Missile, 1,
                                             Abilities::Target::Enemy);
            default:
                break;
        }
        return nullptr;
    }

} // namespace CitiesOfSigmar
