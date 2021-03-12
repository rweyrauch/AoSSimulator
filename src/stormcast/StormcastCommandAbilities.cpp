/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "StormcastEternalsPrivate.h"
#include "CommandAbility.h"

namespace StormcastEternals {

    class HolyCrusaders : public CommandAbility {
    public:
        explicit HolyCrusaders(Unit *source);

    protected:
        bool apply(Unit *target) override;

        bool apply(double x, double y) override { return false; }
    };

    HolyCrusaders::HolyCrusaders(Unit *source) :
            CommandAbility(source, "Holy Crusaders", 18, 9, Phase::Hero) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_targetKeywords = {HALLOWED_KNIGHTS};
        m_effect = Abilities::EffectType::Buff;
    }

    bool HolyCrusaders::apply(Unit *target) {
        if (target == nullptr)
            return false;

        target->buffModifier(Attribute::Run_Distance, 1, {Phase::Hero, m_round + 1, m_source->owningPlayer()});
        target->buffModifier(Attribute::Charge_Distance, 1, {Phase::Hero, m_round + 1, m_source->owningPlayer()});
        target->buffMovement(MovementRule::Run_And_Charge, true, {Phase::Hero, m_round + 1, m_source->owningPlayer()});

        return true;
    }

    CommandAbility *CreateCommandAbility(Command which, Unit *source) {
        switch (which) {
            case Command::Soul_Of_The_Stormhost:
                return nullptr;
            case Command::Holy_Crusaders:
                return new HolyCrusaders(source);
            case Command::Righteous_Hatred:
                return new BuffModifierCommandAbility(source, "Righteous Hatred", 18, 9, Phase::Combat,
                                                      Attribute::Attacks_Melee, 1, Abilities::Target::SelfAndFriendly,
                                                      std::vector<Keyword>{CELESTIAL_VINDICATORS});
            case Command::Heroes_Of_Another_Age:
                return nullptr;
            case Command::No_Mercy:
                return new BuffRerollCommandAbility(source, "No Mercy", 18, 9, Phase::Hero, Attribute::To_Wound_Melee,
                                                    Rerolls::Ones, Abilities::Target::SelfAndFriendly,
                                                    std::vector<Keyword>{KNIGHTS_EXCELSIOR});
            case Command::Astral_Conjunction:
                return new BuffModifierCommandAbility(source, "Astral Conjunction", 18, 9, Phase::Hero,
                                                      Attribute::Casting_Roll, 1, Abilities::Target::SelfAndFriendly,
                                                      std::vector<Keyword>{CELESTIAL_WARBRINGERS, WIZARD});
            case Command::Rousing_Oratory:
                return new BuffRerollCommandAbility(source, "Rousing Oratory", 18, 9, Phase::Combat,
                                                    Attribute::To_Wound_Melee, Rerolls::Ones,
                                                    Abilities::Target::SelfAndFriendly,
                                                    std::vector<Keyword>{TEMPEST_LORDS});
            case Command::Cut_Off_The_Head:
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }
}