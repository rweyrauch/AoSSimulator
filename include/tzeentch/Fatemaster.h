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

    class Fatemaster : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Fatemaster();

        ~Fatemaster() override = default;

    protected:

        void configure();

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        int toSaveModifier(const Weapon *weapon, const Unit *attacker) const override;

    private:

        Weapon m_glaive,
                m_teethAndHorns;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Hovering Disc of Tzeentch        Yes
// Soulbound Shield                 Yes
// Lord of Fate                     TODO
//

} // namespace Tzeentch

