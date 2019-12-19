/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TREELORDANCIENT_H
#define TREELORDANCIENT_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class TreelordAncient : public SylvanethBase
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 260;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    TreelordAncient();
    ~TreelordAncient() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onStartHero(PlayerId id) override;
    void onWounded() override;
    int getDamageTableIndex() const;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

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
// Impale                           Yes
// Silent Communication             No
// Awakening the Wood               No
// Heed the Spirit-song             No
//

} // namespace Sylvaneth

#endif //TREELORDANCIENT_H