/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHINERAILIFETAKERS_H
#define KHINERAILIFETAKERS_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class KhineraiLifetakers : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    KhineraiLifetakers();
    ~KhineraiLifetakers() override = default;

    bool configure(int numModels);

protected:

    void onBeginTurn(int battleRound) override;
    void onCharged() override;
    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;

private:

    Weapon m_barbedSickle,
        m_barbedSickleHarridynn;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Descend to Battle                No
// Fight and Flight                 No
// Death on the Wind                Yes
// Heartpiercer Shield              Yes
//

} // namespace DaughtersOfKhaine

#endif //KHINERAILIFETAKERS_H