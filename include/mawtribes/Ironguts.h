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

    class Ironguts : public MawtribesBase {
    public:

        static bool AreValid(const ParameterList &parameters);

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Ironguts() = delete;

        ~Ironguts() override;

    protected:

        Ironguts(Mawtribe tribe, int numModels, bool runeMawBearer, bool bellower);

    protected:

        int bellower(const Unit *target);

    private:

        Weapon m_bashingWeapon{Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 3, 3, 3, 0, 2},
                m_bite{Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1},
                m_bashingWeaponGutlord{Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 4, 3, 3, 0, 2};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bellower                         Yes
// Rune Maw Bearer                  TODO
// Down to the Ironguts             TODO
//

} // namespace OgorMawtribes

