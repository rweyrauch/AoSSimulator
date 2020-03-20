/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef AETHERKHEMIST_H
#define AETHERKHEMIST_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class AetherKhemist : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AetherKhemist();

        ~AetherKhemist() override;

        bool configure();

    protected:

        int atmosphericIsolation(const Unit *attacker, const Weapon *weapon, const Unit *target);

    private:

        Weapon m_anatomiser,
                m_instruments;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aetheric Augmentation            TODO
// Atmospheric Isolation            Yes
//

} // namespace KharadronOverlords

#endif //AETHERKHEMIST_H
