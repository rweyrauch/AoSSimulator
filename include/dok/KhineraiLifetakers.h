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

namespace DaughtersOfKhaine {

    class KhineraiLifetakers : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        KhineraiLifetakers();

        ~KhineraiLifetakers() override = default;

        bool configure(int numModels);

    protected:

        void onBeginTurn(int battleRound) override;

        void onCharged() override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        int toSaveModifier(const Weapon *weapon) const override;

    private:

        Weapon m_barbedSickle,
            m_barbedSickleHarridynn;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Descend to Battle                TODO
// Fight and Flight                 TODO
// Death on the Wind                Yes
// Heartpiercer Shield              Yes
//

} // namespace DaughtersOfKhaine

#endif //KHINERAILIFETAKERS_H