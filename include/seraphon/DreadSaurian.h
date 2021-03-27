/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class DreadSaurian : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        DreadSaurian();

        ~DreadSaurian() override;

    protected:

        void configure();

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        void onFriendlyUnitSlain(const Unit *attacker) override;

        size_t getDamageTableIndex() const;

        int terror(const Unit *target);

    private:

        Weapon m_gargantuanJaws,
                m_rakingClaws,
                m_armouredTail;

        lsignal::slot m_terrorSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Glyphs                    TODO
// Obliterating Charge              Yes
// Death Throes                     Yes
// Terror                           Yes
// Roar of Ruin                     TODO
//

} // namespace Seraphon

