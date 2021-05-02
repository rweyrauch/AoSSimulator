/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class VulkiteBerzerkers : public Fyreslayer {
    public:

        enum WeaponOption {
            Handaxe_And_Shield,
            Warpick_And_Shield,
            Paired_Handaxes
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        VulkiteBerzerkers(int points);

        ~VulkiteBerzerkers() override = default;

        bool configure(int numModels, WeaponOption weapons, bool hornOfGrimnir);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int chargeModifier() const override;

        void onRestore() override;

        void onStartCombat(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

        void onCharged() override;

    private:

        bool m_usedBerserkFury = false;
        bool m_berserkFuryActive = false;

        WeaponOption m_weaponOption = Handaxe_And_Shield;
        bool m_hornOfGrimnir = false;

        Weapon m_handaxe,
                m_handaxeKarl,
                m_warpick,
                m_warpickKarl,
                m_throwingAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Horn of Grimnir                  Yes
// Berserk Fury                     Yes
// Fyresteel Handaxes               Yes
// Bladed Slingshield               Yes
//

} // namespace Fyreslayers
