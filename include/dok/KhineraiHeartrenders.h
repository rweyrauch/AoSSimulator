/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHINERAIHEARTRENDERS_H
#define KHINERAIHEARTRENDERS_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine
{

class KhineraiHeartrenders : public DaughterOfKhaine
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 90;
    static const int POINTS_MAX_UNIT_SIZE = 90*4;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels);
    static void Init();

    KhineraiHeartrenders();
    ~KhineraiHeartrenders() override = default;

    bool configure(int numModels);

protected:

    Wounds computeReturnedDamage(const Weapon* weapon, int saveRoll) const override;

private:

    Weapon m_barbedJavelinMissile,
        m_barbedJavelin,
        m_barbedJavelinShrykeMissile,
        m_barbedJavelinShryke;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Descend to Battle                TODO
// Fight and Flight                 TODO
// Death From Above                 TODO
// Heartpiercer Shield              Yes
//

} // namespace DaughtersOfKhaine

#endif //KHINERAIHEARTRENDERS_H