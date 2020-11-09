/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ORRUKWARBOSS_H
#define ORRUKWARBOSS_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Greenskinz {

    class OrrukWarboss : public Unit {
    public:

        enum WeaponOption {
            Boss_Choppa_And_Shield,
            Paired_Boss_Choppas,
            Massive_Choppa,
            Great_Waaagh_Banner
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        OrrukWarboss();

        ~OrrukWarboss() override = default;

        bool configure(WeaponOption weapon, bool warboar);

    protected:

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon) const override;

    private:

        WeaponOption m_weapon = Boss_Choppa_And_Shield;

        Weapon m_bossChoppa,
                m_massiveChoppa,
                m_greatWaaaghBanner,
                m_boarTusks;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Choppa Boss                      Yes
// Great Waaagh! Banner             TODO
// Boss Shield                      Yes
// Waaagh!                          TODO
//

} // namespace Greenskinz

#endif // ORRUKWARBOSS_H