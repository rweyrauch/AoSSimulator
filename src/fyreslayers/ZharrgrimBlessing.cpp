/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/ZharrgrimBlessing.h>

namespace Fyreslayers {

    class PrayerOfAsh : public Prayer {
    public:
        explicit PrayerOfAsh(Unit *priest) :
                Prayer(priest, "Prayer of Ash", 3, 18, 0) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(int prayingRoll, Unit *target) override {
            if (target == nullptr) return false;
            target->buffModifier(Attribute::To_Save_Missile, 1, {Phase::Hero, m_round + 1, m_priest->owningPlayer()});
            target->buffModifier(Attribute::To_Save_Melee, 1, {Phase::Hero, m_round + 1, m_priest->owningPlayer()});
            return true;
        }

        bool apply(int prayingRoll, double x, double y) override { return false; }
    };

    Prayer *CreateZharrgrimBlessing(Blessing which, Unit *priest) {
        switch (which) {
            case Blessing::Molten_Infusion:
                // TODO: Molten_Infusion
                return nullptr;
            case Blessing::Searing_Heat:
                return new BuffModifierPrayer(priest, "Searing Heat", 3, 18, Attribute::To_Hit_Melee, -1,
                                              Abilities::Target::Enemy, {}); // TODO: Also debuff To_Hit_Missile
            case Blessing::Prayer_Of_Ash:
                return new PrayerOfAsh(priest);
            case Blessing::Ember_Storm:
                // TODO: Ember_Storm
                return nullptr;
            case Blessing::Prayer_Of_Grimnirs_Fury:
                // TODO: Prayer_Of_Grimnirs_Fury
                return nullptr;
            case Blessing::Gilded_Claws:
                // TODO: Gilded_Claws
                return nullptr;
            default:
                break;
        }
        return nullptr;
    }

} // namespace Khorne