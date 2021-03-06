/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <ironjawz/Ironjawz.h>

namespace Ironjawz {

    class MegabossOnMawKrusha : public Ironjawz {
    public:

        enum WeaponOption {
            Hacka_And_Choppa = 0,
            Choppa_And_Riptoof_Fist,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        MegabossOnMawKrusha(Warclan warclan, WeaponOption option, MountTrait mountTrait, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~MegabossOnMawKrusha() override = default;

    protected:

        void onWounded() override;

        void onCharged() override;

        void onRestore() override;

        void onStartCombat(PlayerId player) override;

        void onEndCombat(PlayerId player) override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        size_t getDamageTableIndex() const;

        WeaponOption m_weaponOption = Hacka_And_Choppa;
        int m_modelsSlainAtStartOfCombat = 0;

        Weapon m_bellow,
                m_hackaAndChoppa,
                m_ripToofFist,
                m_fistsAndTail;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Destructive Bulk                 Yes
// Rip-toof Fist                    Yes
// Strength from Victory            Yes
// Go on Ladz, Get Stuck In         Yes
//

} // namespace Ironjawz
