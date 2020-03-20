/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SORCERESSBLACKDRAGON_H
#define SORCERESSBLACKDRAGON_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class SorceressOnBlackDragon : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        SorceressOnBlackDragon();

        ~SorceressOnBlackDragon() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        int getDamageTableIndex() const;

        Weapon m_noxiousBreath,
                m_rod,
                m_sword,
                m_lash,
                m_jaws,
                m_claws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blood Sacrifice                  TODO
// Noxious Breath                   Yes
// Bladewind                        TODO
// Command Underlings               TODO
// Inspire Hatred                   TODO
//

} // namespace CitiesOfSigmar

#endif //SORCERESSBLACKDRAGON_H