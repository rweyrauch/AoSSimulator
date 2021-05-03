/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <idonethdeepkin/IdonethDeepkin.h>
#include <Weapon.h>

namespace IdonethDeepkin {

    class IsharannTidecaster : public IdonethDeepkinBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        IsharannTidecaster(Enclave enclave, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~IsharannTidecaster() override = default;

        void setRiptideTarget(Unit* target) { m_riptideTarget = target; }

    protected:

        void onStartHero(PlayerId player) override;

        Unit* m_riptideTarget = nullptr;

    private:

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Spirit Guardians                 TODO
// The Wide Ethersea                TODO
// Riptide                          Yes
//

} // namespace IdonethDeepkin

