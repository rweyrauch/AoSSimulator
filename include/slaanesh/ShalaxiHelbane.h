/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SHALAXIHELBANE_H
#define SHALAXIHELBANE_H

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

        bool configure(WeaponOption weapon, Lore lore);

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        Wounds applyWoundSave(const Wounds &wounds) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        WeaponOption m_weapon = Shining_Aegis;

        Weapon m_livingWhip,
                m_soulpiercer,
                m_impalingClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Cloak of Constriction            TODO
// Delicate Precision               Yes
// Irresistible Challenge           TODO
// Living Whip                      TODO
// Shining Aegis                    Yes
// The Killing Stroke               TODO
// Refine Senses                    TODO
//

} // Slannesh

#endif //SHALAXIHELBANE_H