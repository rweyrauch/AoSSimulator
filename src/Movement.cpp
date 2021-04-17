/*
 * Warhammer 40k battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include "Movement.h"
#include "Board.h"
#include "Roster.h"

void MovementPhase::run(PlayerId player, Board* board) {

    auto roster = board->getPlayerRoster(player);
    if (roster != nullptr) {
        for (auto& unit : *roster) {
            unit->movement(player);
        }
    }
}
