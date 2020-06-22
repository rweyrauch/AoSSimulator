/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LUMINETHPRIVATE_H
#define LUMINETHPRIVATE_H

#include <lumineth/LuminethRealmLords.h>
#include <array>
#include "../AoSSimPrivate.h"

namespace LuminethRealmLords {

    constexpr std::array<int, 7> g_commandTraits = {
            to_integer(CommandTrait::None),
    };

    constexpr std::array<int, 7> g_artefacts = {
            to_integer(Artefact::None),
    };


    constexpr std::array<int, 7> g_lore = {
            to_integer(Lore::None),
    };

    class PowerOfHysh : public Spell {
    public:
        explicit PowerOfHysh(Unit *caster);

        Result cast(Unit *target, int round) override;

        Result cast(double x, double y, int round) override { return Failed; }

    };

} // namespace LuminethRealmLords

#endif // LUMINETHPRIVATE_H
