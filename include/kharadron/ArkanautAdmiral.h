/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class ArkanautAdmiral : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ArkanautAdmiral(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~ArkanautAdmiral() override = default;

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon m_pistol,
                m_skalfhammer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// If You Want A Job Done...        Yes
// Protect the Admiral!             TODO
// Master of the Skies              TODO
// On My Mark, Fire!                TODO
// Repel Boarders!                  TODO
// Up And At Them!                  TODO
//

} // namespace KharadronOverlords

