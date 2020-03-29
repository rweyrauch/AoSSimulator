/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BJORGENTHUNDRIK_H
#define BJORGENTHUNDRIK_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class BjorgenThundrik : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        BjorgenThundrik();

        ~BjorgenThundrik() override = default;

        bool configure();

    protected:

    private:

        Weapon  m_anatomiser,
                m_instruments;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aetheric Augmentation            TODO
// Atmospheric Isolation            TODO
//

} // namespace KharadronOverlords

#endif //BJORGENTHUNDRIK_H
