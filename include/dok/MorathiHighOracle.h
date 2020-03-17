/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORATHIORACLE_H
#define MORATHIORACLE_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class MorathiHighOracleOfKhaine : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    MorathiHighOracleOfKhaine();
    ~MorathiHighOracleOfKhaine() override = default;

    bool configure();

protected:


private:

    Weapon m_heartrender,
        m_wings;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Monstrous Transformation         TODO
// The Truth Revealed               TODO
// The Iron Heart of Khaine         TODO
// Sorceress Supreme                TODO
// Enchanting Beauty                TODO
// Arnzipal's Black Horror          TODO
// Worship Through Bloodshed        TODO
//

} // namespace DaughtersOfKhaine

#endif //MORATHIORACLE_H