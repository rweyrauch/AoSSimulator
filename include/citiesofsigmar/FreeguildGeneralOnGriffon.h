/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEGENERALGRIFFON_H
#define FREEGENERALGRIFFON_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class FreeguildGeneralOnGriffon : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            RuneSword,
            Greathammer,
            Lance
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        FreeguildGeneralOnGriffon();

        ~FreeguildGeneralOnGriffon() override;

        bool configure(WeaponOption weapon, bool hasShield);

    protected:

        int toSaveModifier(const Weapon *weapon) const override;

        int weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int runModifier() const override;

        int chargeModifier() const override;

        void onWounded() override;

        void onRestore() override;

        int piercingBloodroar(const Unit *target);

    private:

        int getDamageTableIndex() const;

        bool m_shield = false;

        Weapon m_runesword,
                m_greathammer,
                m_lance,
                m_claws,
                m_beak;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Charging Lance                   Yes
// Freeguild Shield                 Yes
// Skilled Rider                    Yes
// Piercing Bloodroar               Yes
// Rousing Battle Cry               TODO
//

} // namespace CitiesOfSigmar

#endif //FREEGENERALGRIFFON_H