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

    class SorceressOnBlackDragon : public CitizenOfSigmar {
    public:

        enum WeaponOption {
            Witch_Rod,
            Darkling_Sword
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        SorceressOnBlackDragon(City city, Lore lore, WeaponOption weaponOption, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~SorceressOnBlackDragon() override = default;

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartHero(PlayerId player) override;

        int castingModifier() const override;

    private:

        size_t getDamageTableIndex() const;

        Weapon m_noxiousBreath,
                m_rod,
                m_sword,
                m_lash,
                m_jaws,
                m_claws;

        int m_bloodSacrificeMod = 0;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blood Sacrifice                  Yes
// Noxious Breath                   Yes
// Bladewind                        Yes
// Command Underlings               Yes
// Inspire Hatred                   Yes
//

} // namespace CitiesOfSigmar
