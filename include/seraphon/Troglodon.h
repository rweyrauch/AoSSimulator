/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TROGLODON_H
#define TROGLODON_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class Troglodon : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 140;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    Troglodon();
    ~Troglodon() override = default;

    bool configure();

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_spittle,
        m_jaws,
        m_forelimbs,
        m_rod;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Oracle of the Slann              No
// Regeneration                     No
// Terror                           No
// Venemous Spittle                 No
// Drawn to the Screams             No
// Comet's Call                     No
//

} // namespace Seraphon

#endif //TROGLODON_H