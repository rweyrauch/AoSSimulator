/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class PlagueMonks : public Skaventide {
    public:

        enum WeaponOptions {
            Paired_Foetid_Blades,
            Foetid_Blade_And_Woe_Stave
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        PlagueMonks(int points);

        ~PlagueMonks() override = default;

        bool configure(int numModels, WeaponOptions weapons, int bannerBearers, int harbingers);

    protected:

        void onStartHero(PlayerId player) override;

        int runModifier() const override;

        int chargeModifier() const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        void onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) override;

    private:

        WeaponOptions m_weaponOption = Paired_Foetid_Blades;
        int m_numBanners = 0;
        int m_numHarbingers = 0;

        Weapon m_pairedBlades,
                m_bladeAndStave;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Book of Woes                     Yes
// Standard Bearer                  Yes
// Plague Harbingers                Yes
// Foetid Weapons                   Yes
// Frenzied Assault                 Yes
//

} // namespace Skaven
