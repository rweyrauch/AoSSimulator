/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODWRACKSHRINE_H
#define BLOODWRACKSHRINE_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class BloodwrackShrine : public DaughterOfKhaine
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    BloodwrackShrine();
    ~BloodwrackShrine() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;

    void onCharged() override;
    void onStartHero(PlayerId player) override;

    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    int getDamageTableIndex() const;

    Weapon m_bloodwrackStare,
        m_whisperclaw,
        m_tailOfSerpents,
        m_bloodwrackSpear,
        m_goadstaves;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Bladed Impact                    Yes
// Bloodwrack Stare                 Yes
// Aura of Agony                    Yes
// Enfeebling Foe                   TODO
//

} // namespace DaughtersOfKhaine

#endif //BLOODWRACKSHRINE_H