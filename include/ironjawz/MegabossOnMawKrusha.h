/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MEGABOSS_MAWKRUSHA_H
#define MEGABOSS_MAWKRUSHA_H

#include <ironjawz/Ironjawz.h>

namespace Ironjawz
{

class MegabossOnMawKrusha : public Ironjawz
{
public:
    static const int BASESIZE = 100;
    static const int WOUNDS = 15;
    static const int POINTS_PER_UNIT = 460;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    MegabossOnMawKrusha();

    ~MegabossOnMawKrusha() override = default;

    int move() const override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override;

protected:

    void onWounded() override;

private:

    int getDamageTableIndex() const;

    Weapon m_bellow,
        m_hackaAndChoppa,
        m_ripToofFist,
        m_fistsAndTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Destructive Bulk                 No
// Rip-toof Fist                    No
// Strength from Victory            No
// Go on Ladz, Get Stuck In         No
//

} // namespace Ironjawz

#endif //MEGABOSS_MAWKRUSHA_H