/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class Ylthari : public SylvanethBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Ylthari(Lore lore, bool isGeneral);

        Ylthari() = delete;

        ~Ylthari() override = default;

    private:

        Weapon m_briarStaff,
                m_thorns,
                m_snappingMandibles;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Vigour and Wrath                 TODO
// The Reaping                      Yes
//

} // namespace Sylvaneth
