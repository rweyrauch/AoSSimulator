/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon {

    class SaurusScarVeteranOnCarnosaur : public SeraphonBase {
    public:

        enum WeaponOption {
            Celestite_Warblade,
            Celestite_Warspear,
            Celestite_Greatblade
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        SaurusScarVeteranOnCarnosaur();

        ~SaurusScarVeteranOnCarnosaur() override;

        bool configure(WeaponOption option);

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int terror(const Unit *target);

    private:

        Weapon m_warblade,
                m_warspear,
                m_greatblade,
                m_forelimbs,
                m_jaws;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blood Frenzy                     TODO
// Celestite Warspear               Yes
// Cold Ferocity                    Yes
// Pinned Down                      Yes
// Terror                           Yes
// Saurian Savagery                 TODO
//

} // namespace Seraphon
