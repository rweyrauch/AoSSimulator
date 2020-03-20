/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COVENTHRONE_H
#define COVENTHRONE_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death {

    class CovenThrone : public LegionOfNagashBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        CovenThrone();

        ~CovenThrone() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

    private:

        Weapon m_bite,
            m_stiletto,
            m_poniards,
            m_etherealWeapons;

        static bool s_registered;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Frightful Touch                  TODO
// Scrying Pool                     TODO
// The Hunger                       TODO
// Deathly Invocation               TODO
// Beguile                          TODO
// Tactful Insight                  TODO
//

} // namespace Death

#endif // COVENTHRONE_H