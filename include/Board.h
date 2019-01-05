/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_BOARD_H
#define WARHAMMERSIM_BOARD_H

#include <vector>
#include "TerrainFeature.h"

class Board
{
public:
    Board() = default;

private:
    std::vector<TerrainFeature> m_features;
};


#endif //WARHAMMERSIM_BOARD_H
