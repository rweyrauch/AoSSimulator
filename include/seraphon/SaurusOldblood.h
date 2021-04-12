/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SaurusOldblood : public SeraphonBase {
    public:

        enum WeaponOption {
            Celestite_Maul,
            Celestite_Warblade,
            Celestite_Warspear,
            Celestite_Greatblade
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        SaurusOldblood(WayOfTheSeraphon way, Constellation constellation, WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral);

        SaurusOldblood() = delete;

        ~SaurusOldblood() override = default;

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon  m_maul{Weapon::Type::Melee, "Celestite Maul", 1, 3, 4, 3, -1, RAND_D3},
                m_warblade{Weapon::Type::Melee, "Celestite Warblade", 1, 5, 3, 3, 0, 1},
                m_warspear{Weapon::Type::Melee, "Celestite Warspear", 2, 4, 3, 3, 0, 1},
                m_greatblade{Weapon::Type::Melee, "Celestite Greatblade", 1, 3, 4, 3, -1, 2},
                m_jaws{Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Wrath of the Seraphon            TODO
// Cold Ferocity                    Yes
//

} // namespace Seraphon
