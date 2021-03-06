/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class KhineraiHeartrenders : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        KhineraiHeartrenders(Temple temple, int numModels, int points);

        ~KhineraiHeartrenders() override = default;

    protected:

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onRestore() override;

    private:

        int m_setupInRound = 0;

        Weapon m_barbedJavelinMissile,
                m_barbedJavelin,
                m_barbedJavelinShrykeMissile,
                m_barbedJavelinShryke;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fight and Flight                 TODO
// Death From Above                 Partial/TODO
// Heartpiercer Shield              Yes
//

} // namespace DaughtersOfKhaine
