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

    class FrostSabres : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FrostSabres() = delete;

        ~FrostSabres() override = default;

    protected:

        FrostSabres(Mawtribe tribe, int numModels, int points);

    protected:

        int chargeModifier() const override;

        int braveryModifier() const override;

    private:

        Weapon m_fangs{Weapon::Type::Melee, "Elongated Fangs", 1, 3, 4, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Their Master's Voice             Yes
//

} // namespace OgorMawtribes

