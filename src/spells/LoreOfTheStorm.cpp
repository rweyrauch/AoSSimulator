/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <cfloat>
#include <spells/LoreOfTheStorm.h>
#include <Unit.h>
#include "magic_enum.hpp"

namespace StormcastEternals {

    std::string ToString(Lore which) {
        return std::string(magic_enum::enum_name(which));
    }

    bool FromString(const std::string &enumString, Lore &outLore) {
        auto lore = magic_enum::enum_cast<Lore>(enumString);
        outLore = lore.value();
        return lore.has_value();
    }

    DamageSpell *CreateLightningBlast(Unit *caster) {
        return new DamageSpell(caster, "Lightning Blast", 5, DBL_MAX, RAND_D3);
    }

    AreaOfEffectSpell *CreateStarfall(Unit *caster) {
        return new AreaOfEffectSpell(caster, "Starfall", 5, 12.0, 3.0, 1, 4);
    }

    class Thundershock : public AreaOfEffectSpell {
    public:
        explicit Thundershock(Unit *caster) :
                AreaOfEffectSpell(caster, "Thundershock", 6, 0.0, 6.0, 1, 4) {}

    protected:
        void secondaryEffect(Unit *target, int round) const override;
    };

    void Thundershock::secondaryEffect(Unit *target, int round) const {
        if (target) {
            target->buffModifier(ToHitMelee, -1, {Phase::Hero, round + 1, m_caster->owningPlayer()});
            target->buffModifier(ToHitMissile, -1, {Phase::Hero, round + 1, m_caster->owningPlayer()});
        }
    }

    AreaOfEffectSpell *CreateThundershock(Unit *caster) {
        return new class Thundershock(caster);
    }

    AreaOfEffectSpell *CreateStormcaller(Unit *caster) {
        return new AreaOfEffectSpell(caster, "Stormcaller", 7, 0.0, DBL_MAX, RAND_D3, 6);
    }

    Spell *CreateAzyriteHalo(Unit *caster) {
        return nullptr;
    }

    AreaOfEffectSpell *CreateChainLightning(Unit *caster) {
        return nullptr;
    }

    Spell *CreateTerrifyingAspect(Unit *caster) {
        return nullptr;
    }

    Spell *CreateCelestialBlades(Unit *caster) {
        return new BuffModifierSpell(caster, "Celestial Blades", 5, 18.0, ToWoundMelee, 1, true);
    }

    Spell *CreateSpeedOfLightning(Unit *caster) {
        return new BuffRerollSpell(caster, "Speed of Lightning", 5, 9.0, ChargeDistance, RerollFailed, true);
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
            case Lore::Speed_of_Lightning:
                return CreateSpeedOfLightning(caster);
            default:
                return nullptr;
        }
    }

} // namespace StormcastEternals
