/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BATSWARM_H
#define BATSWARM_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {
    class BatSwarms : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BatSwarms();

        ~BatSwarms() override;

        bool configure(int numModels);

    protected:

        int cloudOfHorrors(const Unit* attacker, const Weapon* weapon, const Unit* target);

    private:

        Weapon m_teeth;

        lsignal::slot m_cloudSlot;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cloud of Horror                  Yes
// Blood Suckers                    TODO
//

} //namespace Death

#endif //BATSWARM_H