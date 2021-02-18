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

    class TzaangorShaman : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        TzaangorShaman();

        ~TzaangorShaman() override;

        bool configure();

    protected:

        int visionsToHitMod(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_staff,
                m_dagger,
                m_teethAndHorns;

        lsignal::slot m_visionsSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Sorcerous Elixir                 TODO
// Visions of the Future            Yes
// Visions of the Past              Yes
// Boon of Mutation                 Yes
//

} // namespace Tzeentch

