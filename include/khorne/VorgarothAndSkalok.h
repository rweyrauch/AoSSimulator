/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VORGAROTH_H
#define VORGAROTH_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class VorgarothAndSkalok : public KhorneBase
{
public:

    static const int BASESIZE = 280; // x210 oval
    static const int WOUNDS = 30;
    static const int POINTS_PER_UNIT = 1200;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    VorgarothAndSkalok();
    ~VorgarothAndSkalok() override = default;

    bool configure();
    int move() const override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int getDamageTableIndex() const;
    void onWounded() override;

    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    void onCharged() override;

private:

    Weapon m_balefire,
        m_skullCleaverAxeOfKhorne,
        m_evisceratingClaws,
        m_cavernousJaws,
        m_brassPlatedTail;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Monstrous Trophies               Yes
// Crushing Bulk                    Yes
// Eternal Hunt                     No
// Fettered Servitude               No
// Fuelled by Death                 No
// Wings of Fury                    No
//

} // namespace Khorne

#endif //VORGAROTH_H