/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Prayer.h>
#include <array>

namespace Khorne {

    enum class BloodBlessingsOfKhorne : int {
        Bronzed_Flesh,
        Blood_Sacrifice,
        Resanguination,
        Brazen_Fury,
        Killing_Frenzy,
        Spellbane_Hex,
    };

    std::string ToString(BloodBlessingsOfKhorne which);

    bool FromString(const std::string &enumString, BloodBlessingsOfKhorne &outBlessing);

    Prayer *CreateBloodBlessingsOfKhorne(BloodBlessingsOfKhorne which, Unit *priest);

} // namespace Khorne
