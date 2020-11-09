/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SAURUSOLDBLOOD_H
#define SAURUSOLDBLOOD_H

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

        SaurusOldblood();

        ~SaurusOldblood() override = default;

        bool configure(WeaponOption option);

    protected:

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

    private:

        Weapon m_maul,
                m_warblade,
                m_warspear,
                m_greatblade,
                m_jaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Wrath of the Seraphon            TODO
// Cold Ferocity                    Yes
//

} // namespace Seraphon

#endif //SAURUSOLDBLOOD_H