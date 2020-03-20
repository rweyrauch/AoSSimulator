/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FIREBELLY_H
#define FIREBELLY_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class Firebelly : public MawtribesBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Firebelly();

        ~Firebelly() override = default;

        bool configure();

    protected:

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_fireBreath,
            m_hammer;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fire Breath                      Yes
// Cascading Fire-cloak             TODO
//

} // namespace OgorMawtribes

#endif //FIREBELLY_H