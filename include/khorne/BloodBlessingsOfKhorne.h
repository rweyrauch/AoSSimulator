/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODBLESSINGSOFKHORNE_H
#define BLOODBLESSINGSOFKHORNE_H

#include <Prayer.h>

enum class BloodBlessingsOfKhorne : int {
    None = 0,
    BronzedFlesh,
    BloodSacrifice,
    Resanguination,
    BrazenFury,
    KillingFrenzy,
    SpellbaneHex,
};

std::string ToString(BloodBlessingsOfKhorne which);

bool FromString(const std::string &enumString, BloodBlessingsOfKhorne &outBlessing);

Prayer *CreateBloodBlessingsOfKhorne(BloodBlessingsOfKhorne which, Unit *priest);

#endif //BLOODBLESSINGSOFKHORNE_H