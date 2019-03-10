/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIRITOFDURTHU_H
#define SPIRITOFDURTHU_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class SpiritOfDurthu : public Unit
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 380;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SpiritOfDurthu();
    ~SpiritOfDurthu() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onStartHero(PlayerId id) override;
    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_verdantBlast,
        m_guardianSword,
        m_massiveImpalingTalons;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Groundshaking Stomp              No
// Impale                           No
// Spirit Paths                     No
// Guardian Sword                   No
// Champions of the Everqueen's Will No
// Verdant Blast                    No
// Solemn Guardian                  No
//

} // namespace Sylvaneth

#endif //SPIRITOFDURTHU_H