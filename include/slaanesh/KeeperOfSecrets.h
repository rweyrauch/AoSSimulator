/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KEEPEROFSECRETS_H
#define KEEPEROFSECRETS_H

#include <Unit.h>
#include <Weapon.h>

namespace Slaanesh
{

class KeeperOfSecrets : public Unit
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 480;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    KeeperOfSecrets();
    ~KeeperOfSecrets() override = default;

    bool configure();
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;

private:

    Weapon m_livingWhip,
        m_ritualKnife,
        m_greatblade,
        m_impalingClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Dark Temptations                 No
// Living Whip                      No
//

} // Slannesh

#endif //KEEPEROFSECRETS_H