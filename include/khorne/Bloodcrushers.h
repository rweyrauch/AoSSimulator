/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODCRUSHERS_H
#define BLOODCRUSHERS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Bloodcrushers : public KhorneBase
{
public:
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = 560;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Bloodcrushers();
    ~Bloodcrushers() override = default;

    bool configure(int numModels, bool iconBearer, bool hornblowers);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onCharged() override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

private:

    bool m_iconBearer = false;
    bool m_hornblower = false;

    Weapon m_hellblade,
        m_hellbladeHunter,
        m_brazenHooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Hornblower                       No
// Icon Bearer                      No
// Decapitating Blow                Yes
// Murderous Charge                 Yes
//

} // namespace Khorne

#endif //BLOODCRUSHERS_H