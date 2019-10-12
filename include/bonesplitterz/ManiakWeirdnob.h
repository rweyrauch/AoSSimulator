/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef MANIAKWEIRDNOB_H
#define MANIAKWEIRDNOB_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz
{

class ManiakWeirdnob : public Bonesplitterz
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    ManiakWeirdnob();
    ~ManiakWeirdnob() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_bonebeastStaff,
        m_tusksAndHooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Tusker Charge                    No
// Weird Squig                      No
// Bone Spirit                      No
//

} // namespace Bonesplitterz

#endif //MANIAKWEIRDNOB_H