/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THANQUALONBONERIPPER_H
#define THANQUALONBONERIPPER_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class ThanquolOnBoneripper : public Skaventide
{
public:

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels);
    static void Init();

    ThanquolOnBoneripper();
    ~ThanquolOnBoneripper() override = default;

    bool configure(int numProjectors);

protected:

    Wounds applyWoundSave(const Wounds &wounds) override;
    void onWounded() override;
    void onRestore() override;

    int getDamageTableIndex() const;

private:

    Weapon m_projectors,
        m_staff,
        m_braziers,
        m_blows;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Protection of the Horned Rat     Yes
// Staff of the Horned Rat          TODO
// Warp-amulet                      TODO
// Warpfire Braziers                TODO
// Warpfire Projectors              TODO
// Warpstone Addiction              TODO
// Madness                          TODO
// Power Behind the Throne          TODO

} // namespace Skaven

#endif //THANQUALONBONERIPPER_H