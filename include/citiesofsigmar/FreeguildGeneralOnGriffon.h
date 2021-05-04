/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class FreeguildGeneralOnGriffon : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Rune_Sword,
            Greathammer,
            Lance
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FreeguildGeneralOnGriffon(City city, WeaponOption weaponOption, bool hasShield, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~FreeguildGeneralOnGriffon() override;

    protected:

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int runModifier() const override;

        int chargeModifier() const override;

        void onWounded() override;

        void onRestore() override;

        int piercingBloodroar(const Unit *target);

    private:

        size_t getDamageTableIndex() const;

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
// Rousing Battle Cry               Yes
//

} // namespace CitiesOfSigmar
