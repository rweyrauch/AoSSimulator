/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class LordOfChange : public TzeentchBase {
    public:

        enum WeaponOption {
            Baleful_Sword,
            Rod_Of_Sorcery,
            Curved_Beak_And_Talons
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LordOfChange(ChangeCoven coven, WeaponOption option, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~LordOfChange() override = default;

        int rollCasting(UnmodifiedCastingRoll &unmodifiedRoll) const override;

        int getInfernalGatewayValue() const;

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

    private:

        Weapon m_rodOfSorcery,
                m_staff,
                m_sword,
                m_beakAndTalons;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Master of Magic                  Yes
// Spell-eater                      TODO
// Spell-thief                      TODO
// Infernal Gateway                 Yes
// Beacon of Sorcery                TODO
//

} // namespace Tzeentch

