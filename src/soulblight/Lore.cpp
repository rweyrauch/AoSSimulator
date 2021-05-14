/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "Lore.h"

namespace Soulblight {

    class AmethystinePinions : public Spell {
    public:
        explicit AmethystinePinions(Unit *caster) :
                Spell(caster, "Amethystine Pinions", 5, 1) {
            m_allowedTargets = Abilities::Target::Self;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            target->buffModifier(Attribute::Move_Distance, 5, defaultDuration());
            target->buffMovement(MovementRule::Can_Fly, true, defaultDuration());

            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    class SpiritGale : public Spell {
    public:
        explicit SpiritGale(Unit *caster) :
                Spell(caster, "Spirit Gale", 6, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            Dice::RollResult rolls;
            Dice::RollD6(3, rolls);
            target->applyDamage({0, rolls.rollsGE(5), Wounds::Source::Spell}, m_caster);
            for (int i = 1; i < 7; i++) {
                if (rolls.m_distribution[i] >= 3) {
                    target->buffModifier(Attribute::To_Wound_Melee, -1, defaultDuration());
                    target->buffModifier(Attribute::To_Wound_Missile, -1, defaultDuration());
                }
                if (rolls.m_distribution[i] >= 2) {
                    target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());
                    target->buffModifier(Attribute::To_Hit_Missile, -1, defaultDuration());
                }
            }
            target->buffMovement(MovementRule::Can_Fly, true, defaultDuration());

            return Spell::Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Overwhelming_Dread:
                return new BuffModifierSpell(caster, "Overwhelming Dread", 5, 18,
                                             {{Attribute::To_Hit_Melee, -1}, {Attribute::To_Hit_Missile, -1}, {Attribute::Bravery, -1}},
                                             Abilities::Target::Enemy);
            case Lore::Fading_Vigour: // TODO
                return nullptr;
            case Lore::Spectral_Grasp: // TODO
                return nullptr;
            case Lore::Prison_Of_Grief: // TODO
                return nullptr;
            case Lore::Decrepify:
                return new BuffModifierSpell(caster, "Decrepify", 6, 18,
                                             {{Attribute::To_Wound_Melee, -1}, {Attribute::To_Wound_Missile, -1}, {Attribute::Weapon_Damage_Melee, -1}},
                                             Abilities::Target::Enemy, std::vector<Keyword>{HERO});
            case Lore::Soul_Harvest: // TODO
                return nullptr;
            case Lore::Blades_Of_Shyish:
                return new AreaOfEffectSpell(caster, "Blades of Shyish", 5, 0, 12, 1, 5);
            case Lore::Spirit_Gale:
                return new SpiritGale(caster);
            case Lore::Vile_Transference: // TODO
                return nullptr;
            case Lore::Amethystine_Pinions:
                return new AmethystinePinions(caster);
            case Lore::Soulpike: // TODO
                return nullptr;
            case Lore::Amaranthine_Orb:
                return new LineOfEffectSpell(caster, "Amaranthine Orb", 7, 12, RAND_D6, 4);
            default:
                break;
        }
        return nullptr;
    }

} // namespace Soulblight