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

    WurrgogProphet();
    ~WurrgogProphet() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

private:

    Weapon m_staffAndShiv,
        m_fangedMaw;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Beast Mask                       Yes
// Prophet of Da Waaagh!            No
// Fists of Gork                    No
//

} // namespace Bonesplitterz

#endif //WURRGOGPROPHET_H