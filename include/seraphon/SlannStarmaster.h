/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLANNSTARMASTER_H
#define SLANNSTARMASTER_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class SlannStarmaster : public SeraphonBase
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 260;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    SlannStarmaster();
    ~SlannStarmaster() override = default;

    bool configure();

protected:

    void onStartHero(PlayerId player) override;
    int castingModifier() const override;

private:

    Weapon m_lightning;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Vassal                    TODO
// Foresight                        Yes
// Masters of Order                 Yes
// Comet's Call                     TODO
// Gift from the Heavens            TODO
//

} // namespace Seraphon

#endif //SLANNSTARMASTER_H