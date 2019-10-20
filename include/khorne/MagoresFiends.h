/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MAGORESFIENDS_H
#define MAGORESFIENDS_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class MagoresFiends : public KhorneBase
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int POINTS_PER_UNIT = 40;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    MagoresFiends();
    ~MagoresFiends() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;
    Rerolls chargeRerolls() const override;

private:

    Weapon m_bellyMaw,
        m_daemonicAxe,
        m_goreaxe;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Redhand                          Yes
// Gorefists                        Yes
// No Respite                       No
// Blood Scent                      Yes
//

} // namespace Khorne

#endif //MAGORESFIENDS_H