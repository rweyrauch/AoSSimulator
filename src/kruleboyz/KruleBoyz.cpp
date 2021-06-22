/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Roster.h>
#include <kruleboyz/KruleBoyz.h>
#include <magic_enum.hpp>
#include <Board.h>

#include "kruleboyz/Killaboss.h"
#include "kruleboyz/KillabossOnGnashtoof.h"

namespace Kruleboyz {

    std::string Kruleboyz::ValueToString(const Parameter &parameter) {
        return ParameterValueToString(parameter);
    }

    int Kruleboyz::EnumStringToInt(const std::string &enumString) {
        return 0;
    }


    void Init() {
        Killaboss::Init();
        KillabossOnGnashtoof::Init();
    }

} // namespace Ironjawz

