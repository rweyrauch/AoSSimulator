/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "AgeOfSigmarSim.h"

class Board;

class Phase {

public:

    virtual void run(PlayerId player, Board* board) = 0;

};

