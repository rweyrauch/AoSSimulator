/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GOREGRUNTAS_H
#define GOREGRUNTAS_H

#include <ironjawz/Ironjawz.h>

namespace Ironjawz {

    class OrrukGoreGruntas : public Ironjawz {
    public:

        enum WeaponOption {
            PigIronChoppa = 0,
            JaggedGorehacka,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        OrrukGoreGruntas();

        ~OrrukGoreGruntas() override = default;

        bool configure(int numModels, WeaponOption weapons);

    protected:

    protected:

        Weapon m_pigironChoppa,
                m_jaggedGorehacka,
                m_tusksAndHooves,
                m_bossPigironChoppa,
                m_bossJaggedGorehacka;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Gore-grunta Charge               TODO
//

} // namespace Ironjawz

#endif //GOREGRUNTAS_H