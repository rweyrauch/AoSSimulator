/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class ShalaxiHelbane : public SlaaneshBase {
    public:

        enum WeaponOption {
            Living_Whip,
            Shining_Aegis
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        ShalaxiHelbane();

        ~ShalaxiHelbane() override = default;

        void enableRefineSenses() { m_refinedSensesActive = true; };

    protected:

        void configure(WeaponOption weapon, Lore lore);

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onStartHero(PlayerId player) override {
            if (player == owningPlayer()) {
                m_refinedSensesActive = false;
            }
        }

        void onStartCombat(PlayerId player) override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit* attacker) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

    private:

        bool m_refinedSensesActive = false;

        WeaponOption m_weapon = Shining_Aegis;

        Weapon m_livingWhip,
                m_soulpiercer,
                m_impalingClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cloak of Constriction            Yes
// Delicate Precision               Yes
// Irresistible Challenge           TODO
// Living Whip                      Yes
// Shining Aegis                    Yes
// The Killing Stroke               yES
// Refine Senses                    Yes
//

} // Slannesh
