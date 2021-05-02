/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne {

    class Skullgrinder : public KhorneBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Skullgrinder() = delete;

        ~Skullgrinder() override;

    protected:

        Skullgrinder(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral);

        void onEndCombat(PlayerId player) override;

        int favouredByKhorne(const Unit *unit);

    private:

        Weapon m_brazenAnvil{Weapon::Type::Melee, "Brazen Anvil", 2, 3, 3, 2, -1, 3};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Fiery Anvil                      Yes
// Favoured by Khorne               Yes
//

} // namespace Khorne
