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

namespace StormcastEternals {

    std::string ToString(Lore which) {
        switch (which) {
            case LightningBlast:
                return "Lightning Blast";
            case Starfall:
                return "Starfall";
            case Thundershock:
                return "Thundershock";
            case AzyriteHalo:
                return "Azyrite Halo";
            case ChainLightning:
                return "Chain Lightning";
            case Stormcaller:
                return "Stormcaller";
            case TerrifyingAspect:
                return "Terrifying Aspect";
            case CelestialBlades:
                return "Celestial Blades";
            case SpeedOfLightning:
                return "SpeedOfLightning";
        }
        return "";
    }

    bool FromString(const std::string &enumString, Lore &outLore) {
        bool valid = true;

        if (enumString == "Lightning Blast")
            outLore = LightningBlast;
        else if (enumString == "Starfall")
            outLore = Starfall;
        else if (enumString == "Thundershock")
            outLore = Thundershock;
        else if (enumString == "Azyrite Halo")
            outLore = AzyriteHalo;
        else if (enumString == "Chain Lightning")
            outLore = ChainLightning;
        else if (enumString == "Stormcaller")
            outLore = Stormcaller;
        else if (enumString == "Terrifying Aspect")
            outLore = TerrifyingAspect;
        else if (enumString == "Celestial Blades")
            outLore = CelestialBlades;
        else if (enumString == "Speed of Lightning")
            outLore = SpeedOfLightning;
        else
            valid = false;
        return valid;
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
            case LightningBlast:
                return CreateLightningBlast(caster);
            case Starfall:
                return CreateStarfall(caster);
            case Thundershock:
                return CreateThundershock(caster);
            case AzyriteHalo:
                return CreateAzyriteHalo(caster);
            case ChainLightning:
                return CreateChainLightning(caster);
            case Stormcaller:
                return CreateStormcaller(caster);
            case CelestialBlades:
                return CreateCelestialBlades(caster);
            case TerrifyingAspect:
                return CreateTerrifyingAspect(caster);
            case SpeedOfLightning:
                return CreateSpeedOfLightning(caster);
            default:
                return nullptr;
        }
    }

} // namespace StormcastEternals
