/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ROTIGUS_H
#define ROTIGUS_H

#include <nurgle/Nurgle.h>
#include <Weapon.h>

namespace Nurgle
{

class Rotigus : public NurgleBase
{
public:

    static const int BASESIZE = 130;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 340;

    static Unit* Create(const ParameterList& parameters);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Rotigus();
    ~Rotigus() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onCharged() override;
    int getDamageTableIndex() const;
    Wounds applyWoundSave(const Wounds &wounds) override;
    void onStartHero(PlayerId player) override;

private:

    Weapon m_gnarlrod,
        m_fangedMaw,
        m_nurglings;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Blubber and Bile                 Partial/TODO
// Corpulent Mass                   Yes
// Mountain of Loathsome Flesh      Yes
// Streams of Brackish Filth        Yes
// Deluge of Nurgle                 TODO
//

} // Nurgle

#endif //ROTIGUS_H