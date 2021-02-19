/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/PrayersOfTheStormhosts.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {

    class DivineLight : public Prayer {
    public:
        explicit DivineLight(Unit* priest);

    protected:

        bool apply(int prayingRoll, Unit* target) override;
        bool apply(int prayingRoll, double x, double y) override { return false; }

    };

    DivineLight::DivineLight(Unit *priest) :
        Prayer(priest, "Divine Light", 3, 18) {
        m_allowedTargets = Abilities::Target::Any;
        m_effect = Abilities::EffectType::Buff;
    }

    bool DivineLight::apply(int prayingRoll, Unit *target) {
        if (target == nullptr)
            return false;

        const Duration duration = {Phase::Hero, m_round + 1, m_priest->owningPlayer()};
        if (m_priest->isFriendly(target)) {
            target->buffReroll(Target_To_Hit_Melee, Reroll_Sixes, duration);
            target->buffReroll(Target_To_Hit_Missile,Reroll_Sixes, duration);
        }
        else {
            target->buffReroll(Target_To_Hit_Melee, Reroll_Ones, duration);
            target->buffReroll(Target_To_Hit_Missile, Reroll_Ones, duration);
        }
        return true;
    }

    class BlessWeapons : public Prayer {
    public:
        explicit BlessWeapons(Unit *priest) :
                Prayer(priest, "Bless Weapons", 4, 18) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(int prayingRoll, Unit* target) override {
            if (target == nullptr)
                return false;

            const Duration duration = {Phase::Hero, m_round + 1, m_priest->owningPlayer()};
            target->buffAbility(Extra_Hit_On_Value, 6, duration);

            return true;
        }
        bool apply(int prayingRoll, double x, double y) override { return false; }

    };

    class BolsterFaith : public Prayer {
    public:
        explicit BolsterFaith(Unit* priest) :
                Prayer(priest, "Bolster Faith", 3, 9) {
            m_allowedTargets = Abilities::Target::Friendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(int prayingRoll, Unit* target) override {
            if (target == nullptr)
                return false;
            const Duration duration = {Phase::Hero, m_round + 1, m_priest->owningPlayer()};
            target->buffAbility(Ignore_Battleshock, true, duration);
            return true;
        }
        bool apply(int prayingRoll, double x, double y) override { return false; }
    };

    std::string ToString(PrayersOfTheStormhost which) {
        return std::string(magic_enum::enum_name(which));
    }

    bool FromString(const std::string &enumString, PrayersOfTheStormhost &outPrayer) {
        auto prayer = magic_enum::enum_cast<PrayersOfTheStormhost>(enumString);
        if (prayer.has_value())
            outPrayer = prayer.value();
        return prayer.has_value();
    }

    Prayer *CreateDivineLight(Unit *priest) {
        return new DivineLight(priest);
    }

    Prayer *CreateBlessWeapons(Unit *priest) {
        return new BlessWeapons(priest);
    }

    Prayer *CreateBolsterFaith(Unit *priest) {
        return new BolsterFaith(priest);
    }

    Prayer *CreateAbjuration(Unit *priest) {
        // TODO: Abjuration
        return nullptr;
    }

    Prayer *CreateGodKingsAspect(Unit *priest) {
        // TODO: God Kings Aspect
        return nullptr;
    }

    Prayer *CreateTranslocation(Unit *priest) {
        // TODO: Translocation
        return nullptr;
    }

    Prayer *CreatePrayerOfTheStormhost(PrayersOfTheStormhost which, Unit *priest) {
        switch (which) {
            case PrayersOfTheStormhost::Divine_Light:
                return CreateDivineLight(priest);
            case PrayersOfTheStormhost::Bless_Weapons:
                return CreateBlessWeapons(priest);
            case PrayersOfTheStormhost::Bolster_Faith:
                return CreateBolsterFaith(priest);
            case PrayersOfTheStormhost::Abjuration:
                return CreateAbjuration(priest);
            case PrayersOfTheStormhost::God_Kings_Aspect:
                return CreateGodKingsAspect(priest);
            case PrayersOfTheStormhost::Translocation:
                return CreateTranslocation(priest);
            default:
                return nullptr;
        }
    }

} //namespace StormcastEternals
