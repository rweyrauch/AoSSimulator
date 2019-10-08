/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GORDRAKK_H
#define GORDRAKK_H

#include <ironjawz/Ironjawz.h>

namespace Ironjawz
{

class GordrakkTheFistOfGork : public Ironjawz
{
public:
    static const int BASESIZE = 100;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 540;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    GordrakkTheFistOfGork();

    ~GordrakkTheFistOfGork() override = default;

    int move() const override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override;

protected:

    void onWounded() override;

private:

    int getDamageTableIndex() const;

    Weapon m_bellow,
        m_smasha,
        m_kunnin,
        m_fistsAndTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Massively Destructive Bulk       No
// Kunnin                           No
// Smasha                           No
// Strength from Victory            No
// Voice of Gork                    No
//

} // namespace Ironjawz

#endif //GORDRAKK_H