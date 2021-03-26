/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class KairosFateweaver : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        KairosFateweaver();

        ~KairosFateweaver() override = default;

        bool configure();

        int rollCasting(UnmodifiedCastingRoll& unmodifiedRoll) const override;

        int getGiftOfChangeDamage() const;

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_staff,
                m_beakAndTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Master of Magic                  Yes
// Spell-eater                      TODO
// Oracle of Eternity               TODO
// Gift of Change                   Yes
//

} // namespace Tzeentch

