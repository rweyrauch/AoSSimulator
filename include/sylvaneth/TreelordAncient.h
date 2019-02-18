/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TREELORDANCIENT_H
#define TREELORDANCIENT_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class TreelordAncient : public Unit
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    TreelordAncient();
    ~TreelordAncient() override = default;

    bool configure();

    void hero(PlayerId id) override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_doomTendrilStaff,
        m_sweepingBlows,
        m_massiveImpalingTalons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Groundshaking Stomp              No
// Spirit Paths                     No
// Impale                           No
// Silent Communication             No
// Awakening the Wood               No
// Heed the Spirit-song             No
//

} // namespace Sylvaneth

#endif //TREELORDANCIENT_H