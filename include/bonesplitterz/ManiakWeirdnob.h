/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz {

    class ManiakWeirdnob : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        ManiakWeirdnob(Warclan warclan, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~ManiakWeirdnob() override = default;

    protected:

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

        void onBeginTurn(int battleRound, PlayerId player) override;

        Rerolls castingRerolls() const override;

        Rerolls unbindingRerolls() const override;

    private:

        mutable bool m_usedWeirdSquig = false;

        Weapon m_bonebeastStaff,
                m_tusksAndHooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Tusker Charge                    Yes
// Weird Squig                      Yes
// Bone Spirit                      Yes
//

} // namespace Bonesplitterz
