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

    class FluxmasterHeraldOfTzeentchOnDisc : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FluxmasterHeraldOfTzeentchOnDisc(ChangeCoven coven, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~FluxmasterHeraldOfTzeentchOnDisc() override = default;

        int rollCasting(UnmodifiedCastingRoll &unmodifiedRoll) const override;

    protected:

        void onRestore() override {
            TzeentchBase::onRestore();
            m_usedArcaneTome = false;
        }

    private:

        mutable bool m_usedArcaneTome = false;

        Weapon m_flames,
                m_staff,
                m_dagger,
                m_teethAndHorns;

        static bool s_registered;
    };
//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Tome                      Yes
// Blue Fire of Tzeentch            Yes
//

} // namespace Tzeentch

