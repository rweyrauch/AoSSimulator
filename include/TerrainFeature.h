/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_TERRAINFEATURE_H
#define WARHAMMERSIM_TERRAINFEATURE_H


class TerrainFeature
{
public:
    TerrainFeature() = default;

private:

    float m_x, m_y;
    float m_width, m_depth, m_height;

};


#endif //WARHAMMERSIM_TERRAINFEATURE_H
