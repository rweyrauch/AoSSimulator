/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TREELORD_H
#define TREELORD_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class Treelord : public SylvanethBase
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Treelord();
    ~Treelord() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onStartHero(PlayerId id) override;
    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_strangleroots,
        m_sweepingBlows,
        m_massiveImpalingTalons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Groundshaking Stomp              No
// Impale                           No
// Spirit Path                      No
//

} // namespace Sylvaneth

#endif //TREELORD_H