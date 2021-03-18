/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class Leadbelchers : public MawtribesBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Leadbelchers();

        ~Leadbelchers() override = default;

        bool configure(int numModels);

    protected:

        void onStartShooting(PlayerId player) override;

    private:

        Weapon m_gun,
                m_blow,
                m_bite,
                m_blowThunderfist;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Thunderous Blasts of Hot Metal   Yes
//

} // namespace OgorMawtribes

