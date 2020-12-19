/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDOFPAIN_H
#define LORDOFPAIN_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class LordOfPain : public SlaaneshBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        LordOfPain();

        ~LordOfPain() override = default;

        bool configure();

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_mace;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Share the Pain                   TODO
// Paragon of Depravity             TODO
//

} // Slannesh

#endif //LORDOFPAIN_H