/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/BloodBlessingsOfKhorne.h>

std::string ToString(BloodBlessingsOfKhorne which) {
    switch (which) {
        case BloodBlessingsOfKhorne::None:
            return "None";
        case BloodBlessingsOfKhorne::BronzedFlesh:
            return "Bronzed Flesh";
        case BloodBlessingsOfKhorne::BloodSacrifice:
            return "Blood Sacrifice";
        case BloodBlessingsOfKhorne::Resanguination:
            return "Resanguination";
        case BloodBlessingsOfKhorne::BrazenFury:
            return "Brazen Fury";
        case BloodBlessingsOfKhorne::KillingFrenzy:
            return "Killing Frenzy";
        case BloodBlessingsOfKhorne::SpellbaneHex:
            return "Spellbane Hex";
    }
    return "";
}

bool FromString(const std::string &enumString, BloodBlessingsOfKhorne &outBlessing) {
    bool valid = true;

    if (enumString == "None") outBlessing = BloodBlessingsOfKhorne::None;
    else if (enumString == "Bronzed Flesh") outBlessing = BloodBlessingsOfKhorne::BronzedFlesh;
    else if (enumString == "Blood Sacrifice") outBlessing = BloodBlessingsOfKhorne::BloodSacrifice;
    else if (enumString == "Resanguination") outBlessing = BloodBlessingsOfKhorne::Resanguination;
    else if (enumString == "Brazen Fury") outBlessing = BloodBlessingsOfKhorne::BrazenFury;
    else if (enumString == "Killing Frenzy") outBlessing = BloodBlessingsOfKhorne::KillingFrenzy;
    else if (enumString == "Spellbane Hex") outBlessing = BloodBlessingsOfKhorne::SpellbaneHex;
    else valid = false;

    return valid;
}

Prayer *CreateBloodBlessingsOfKhorne(BloodBlessingsOfKhorne which, Unit *priest) {
    switch (which) {
        case BloodBlessingsOfKhorne::BronzedFlesh:
            return new BuffModifierPrayer(priest, "Bronzed Flesh", 4, 16.0, ToSave, 1, true, 1);
        case BloodBlessingsOfKhorne::BloodSacrifice:
            return new DamagePrayer(priest, "Blood Sacrifice", 4, 8.0, RAND_D3, 1);
        case BloodBlessingsOfKhorne::Resanguination:
            return new HealPrayer(priest, "Resanguination", 4, 16.0, RAND_D3, 1);
        case BloodBlessingsOfKhorne::BrazenFury:
            return nullptr;
        case BloodBlessingsOfKhorne::KillingFrenzy:
            return new BuffModifierPrayer(priest, "Killing Frenzy", 4, 16.0, ToHitMelee, 1, true, 1);
        case BloodBlessingsOfKhorne::SpellbaneHex:
        case BloodBlessingsOfKhorne::None:
            return nullptr;
    }
    return nullptr;
}
