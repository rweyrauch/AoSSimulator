/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/BloodBlessingsOfKhorne.h>
#include <magic_enum.hpp>

namespace Khorne {

    std::string ToString(BloodBlessingsOfKhorne which) {
        auto blessingName = magic_enum::enum_name(which);
        return std::string(blessingName);
    }

    bool FromString(const std::string &enumString, BloodBlessingsOfKhorne &outBlessing) {
        auto blessing = magic_enum::enum_cast<BloodBlessingsOfKhorne>(enumString);
        if (blessing.has_value()) outBlessing = blessing.value();
        return blessing.has_value();
    }

    Prayer *CreateBloodBlessingsOfKhorne(BloodBlessingsOfKhorne which, Unit *priest) {
        switch (which) {
            case BloodBlessingsOfKhorne::Bronzed_Flesh:
                return new BuffModifierPrayer(priest, "Bronzed Flesh", 4, 16.0, ToSave, 1, true, 1);
            case BloodBlessingsOfKhorne::Blood_Sacrifice:
                return new DamagePrayer(priest, "Blood Sacrifice", 4, 8.0, RAND_D3, 1);
            case BloodBlessingsOfKhorne::Resanguination:
                return new HealPrayer(priest, "Resanguination", 4, 16.0, RAND_D3, 1);
            case BloodBlessingsOfKhorne::Brazen_Fury:
                return nullptr;
            case BloodBlessingsOfKhorne::Killing_Frenzy:
                return new BuffModifierPrayer(priest, "Killing Frenzy", 4, 16.0, ToHitMelee, 1, true, 1);
            case BloodBlessingsOfKhorne::Spellbane_Hex:
                return nullptr;
        }
        return nullptr;
    }

} // namespace Khorne