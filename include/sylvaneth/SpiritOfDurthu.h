/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIRITOFDURTHU_H
#define SPIRITOFDURTHU_H

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth
{

class SpiritOfDurthu : public SylvanethBase
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    SpiritOfDurthu();
    ~SpiritOfDurthu() override;

    bool configure();

protected:

    void onStartCombat(PlayerId id) override;
    void onStartHero(PlayerId id) override;
    void onWounded() override;
    int getDamageTableIndex() const;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    int championOfTheEverqueensWill(const Unit* target);

private:

    Weapon m_verdantBlast,
        m_guardianSword,
        m_massiveImpalingTalons;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Groundshaking Stomp              No
// Impale                           Yes
// Spirit Paths                     No
// Champions of the Everqueen's Will Yes
// Wrathful Guardian                Yes
//

} // namespace Sylvaneth

#endif //SPIRITOFDURTHU_H