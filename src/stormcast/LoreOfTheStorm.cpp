/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <stormcast/LoreOfTheStorm.h>
#include <Unit.h>
#include <Board.h>
#include "magic_enum.hpp"

namespace StormcastEternals {

    class Thundershock : public AreaOfEffectSpell {
    public:
        explicit Thundershock(Unit *caster) :
                AreaOfEffectSpell(caster, "Thundershock", 6, 0, 6, 1, 4) {}

    protected:
        void secondaryEffect(Unit *target, int round) const override;
    };

    void Thundershock::secondaryEffect(Unit *target, int round) const {
        if (target) {
            target->buffModifier(To_Hit_Melee, -1, defaultDuration());
            target->buffModifier(To_Hit_Missile, -1, defaultDuration());
        }
    }

    class ChainLightning : public Spell {
    public:
        explicit ChainLightning(Unit *caster);

    protected:

        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override;
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }

    };

    ChainLightning::ChainLightning(Unit *caster) :
            Spell(caster, "Chain Lightning", 7, 24) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::AreaOfEffectDamage;
    }

    Spell::Result ChainLightning::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);

        auto units = Board::Instance()->getUnitsWithin(target, target->owningPlayer(), 3.0);
        for (auto unit : units) {
            if (Dice::RollD6() >= 4) {
                unit->applyDamage({0, 1, Wounds::Source::Spell}, m_caster);
            }
        }
        return Spell::Result::Success;
    }


    std::string ToString(Lore which) {
        return std::string(magic_enum::enum_name(which));
    }

    bool FromString(const std::string &enumString, Lore &outLore) {
        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value())
            outLore = lore.value();
        return lore.has_value();
    }


    DamageSpell *CreateLightningBlast(Unit *caster) {
        return new DamageSpell(caster, "Lightning Blast", 5, INT32_MAX, RAND_D3);
    }

    AreaOfEffectSpell *CreateStarfall(Unit *caster) {
        return new AreaOfEffectSpell(caster, "Starfall", 5, 12, 3, 1, 4);
    }

    AreaOfEffectSpell *CreateThundershock(Unit *caster) {
        return new Thundershock(caster);
    }

    AreaOfEffectSpell *CreateStormcaller(Unit *caster) {
        return new AreaOfEffectSpell(caster, "Stormcaller", 7, 0, INT32_MAX, RAND_D3, 6);
    }

    Spell *CreateAzyriteHalo(Unit *caster) {
        return nullptr;
    }

    Spell *CreateChainLightning(Unit *caster) {
        return new ChainLightning(caster);
    }

    Spell *CreateTerrifyingAspect(Unit *caster) {
        return nullptr;
    }

    Spell *CreateCelestialBlades(Unit *caster) {
        return new BuffModifierSpell(caster, "Celestial Blades", 5, 18, To_Wound_Melee, 1, Abilities::Target::Friendly);
    }

    Spell *CreateSpeedOfLightning(Unit *caster) {
        return new BuffRerollSpell(caster, "Speed of Lightning", 5, 9, Charge_Distance, Reroll_Failed, Abilities::Target::Friendly);
    }

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case Lore::Lightning_Blast:
                return CreateLightningBlast(caster);
            case Lore::Starfall:
                return CreateStarfall(caster);
            case Lore::Thundershock:
                return CreateThundershock(caster);
            case Lore::Azyrite_Halo:
                return CreateAzyriteHalo(caster);
            case Lore::Chain_Lightning:
                return CreateChainLightning(caster);
            case Lore::Stormcaller:
                return CreateStormcaller(caster);
            case Lore::Celestial_Blades:
                return CreateCelestialBlades(caster);
            case Lore::Terrifying_Aspect:
                return CreateTerrifyingAspect(caster);
            case Lore::Speed_Of_Lightning:
                return CreateSpeedOfLightning(caster);
            default:
                return nullptr;
        }
    }

} // namespace StormcastEternals
