/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <lumineth/LuminethRealmLords.h>
#include <Weapon.h>

namespace LuminethRealmLords {

    class EllaniaAndEllathor : public LuminethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        EllaniaAndEllathor(Lore lore, bool isGeneral);

        ~EllaniaAndEllathor() override = default;

    protected:

    private:

        Weapon m_altairi{Weapon::Type::Melee, "Altairi", 1, 4, 2, 3, -2, 1},
            m_dianaer{Weapon::Type::Melee, "Dianaer", 1, 2, 3, 3, -1, RAND_D3},
            m_talons{Weapon::Type::Melee, "Moonbright Talons", 1, 2, 3, 3, -1, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aspect of Celennar               TODO
// Realm Wanderers                  TODO
// Altairi                          TODO
// Sudden Translocation             TODO
// Salvation of Hysh                TODO
//

} // namespace LuminethRealmLords

