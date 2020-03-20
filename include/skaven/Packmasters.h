/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PACKMASTERS_H
#define PACKMASTERS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class Packmasters : public Skaventide
{
public:

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    Packmasters();
    ~Packmasters() override;

    bool configure(int numModels, int numCatchers);

protected:

    int crackTheWhip(const Unit* attacker, const Weapon* weapon, const Unit* target);
    int crackTheWhipBravery(const Unit* unit);

private:

    Weapon m_whip,
        m_blade,
        m_catcher;

    lsignal::slot m_whipSlot, m_whipBraverySlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Crack the Whip                   Yes
//

} // namespace Skaven

#endif //PACKMASTERS_H