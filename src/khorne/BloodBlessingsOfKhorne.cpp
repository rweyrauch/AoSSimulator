/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/BloodBlessingsOfKhorne.h>
#include "Unit.h"
#include <magic_enum.hpp>

namespace Khorne {

    class BronzedFlesh : public Prayer {
    public:
        explicit BronzedFlesh(Unit *priest) :
                Prayer(priest, "Bronzed Flesh", 4, 16, 1) {
            m_allowedTargets = Abilities::Target::Friendly;
            m_effect = Abilities::EffectType::Buff;
            m_targetKeywords = {KHORNE};
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

    std::string ToString(BloodBlessingsOfKhorne which) {
        auto blessingName = magic_enum::enum_name(which);
        return std::string(blessingName);
    }

    bool FromString(const std::string &enumString, BloodBlessingsOfKhorne &outBlessing) {
        auto blessing = magic_enum::enum_cast<BloodBlessingsOfKhorne>(enumString);
        if (blessing.has_value())
            outBlessing = blessing.value();
        return blessing.has_value();
    }

    Prayer *CreateBloodBlessingsOfKhorne(BloodBlessingsOfKhorne which, Unit *priest) {
        switch (which) {
            case BloodBlessingsOfKhorne::Bronzed_Flesh:
                return new BronzedFlesh(priest);
            case BloodBlessingsOfKhorne::Blood_Sacrifice:
                return new DamagePrayer(priest, "Blood Sacrifice", 4, 8, RAND_D3, 1);
            case BloodBlessingsOfKhorne::Resanguination:
                return new HealPrayer(priest, "Resanguination", 4, 16, RAND_D3, 1);
            case BloodBlessingsOfKhorne::Brazen_Fury:
                // TODO: Brazen_Fury
                return nullptr;
            case BloodBlessingsOfKhorne::Killing_Frenzy:
                return new BuffModifierPrayer(priest, "Killing Frenzy", 4, 16, Attribute::To_Hit_Melee, 1,
                                              Abilities::Target::SelfAndFriendly, 1);
            case BloodBlessingsOfKhorne::Spellbane_Hex:
                // TODO: Spellbane_Hez
                return nullptr;
        }
        return nullptr;
    }

} // namespace Khorne