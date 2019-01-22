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

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    KhineraiLifetakers();
    ~KhineraiLifetakers() override
    {
        delete m_pBarbedSickle;
        delete m_pBarbedSickleHarridynn;
    }

    bool configure(int numModels);

protected:

    void onBeginTurn(int battleRound) override;
    void onCharged() override;
    Wounds computeReturnedDamage(const Weapon* weapon, const Dice::RollResult& saveRolls) const override;

protected:

    Weapon* m_pBarbedSickle = nullptr;
    Weapon* m_pBarbedSickleHarridynn = nullptr;

private:

    static Weapon s_barbedSickle,
        s_barbedSickleHarridynn;

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