/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef WURRGOGPROPHET_H
#define WURRGOGPROPHET_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz
{

class WurrgogProphet : public Bonesplitterz
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };

    WurrgogProphet();
    ~WurrgogProphet() override = default;

    bool configure();

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;
    void onStartHero(PlayerId playerId) override;

private:

    Weapon m_staffAndShiv,
        m_fangedMaw;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Beast Mask                       Yes
// Prophet of Da Waaagh!            Yes
// Fists of Gork                    TODO
//

} // namespace Bonesplitterz

#endif //WURRGOGPROPHET_H