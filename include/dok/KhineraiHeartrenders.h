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
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    KhineraiHeartrenders();
    ~KhineraiHeartrenders() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

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
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Descend to Battle                No
// Fight and Flight                 No
// Death From Above                 No
// Heartpiercer Shield              Yes
//

} // namespace DaughtersOfKhaine

#endif //KHINERAIHEARTRENDERS_H