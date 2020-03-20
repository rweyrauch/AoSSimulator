/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LIEGEKAVALOS_H
#define LIEGEKAVALOS_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers {

    class LiegeKavalos : public OssiarchBonereaperBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        LiegeKavalos();

        ~LiegeKavalos() override = default;

        bool configure();

    protected:


    private:

        Weapon m_blade,
                m_shield,
                m_hoovesAndTeeth;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Unstoppable Charge               TODO
// Endless Duty                     TODO
//

} // namespace OssiarchBonereapers

#endif //LIEGEKAVALOS_H
