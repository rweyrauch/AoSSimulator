/*
 * Warhammer 40k battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "Phase.h"

class HeroPhase : public Phase {
public:

    void run(PlayerId player, Board* board) override;

};

