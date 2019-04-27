/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SHALAXIHELBANE_H
#define SHALAXIHELBANE_H

#include <Unit.h>
#include <Weapon.h>

namespace Slaanesh
{

class ShalaxiHelbane : public Unit
{
public:

    static const int BASESIZE = 100;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 340;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    ShalaxiHelbane();
    ~ShalaxiHelbane() override = default;

    bool configure();
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;

private:

    Weapon m_livingWhip,
        m_soulpiercer,
        m_impalingClaws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Cloak of Constriction            No
// Delicate Precision               No
// Irresistible Challenge           No
// Living Whip                      No
// Shining Aegis                    No
// The Killing Stroke               No
// Refine Senses                    No
//

} // Slannesh

#endif //SHALAXIHELBANE_H