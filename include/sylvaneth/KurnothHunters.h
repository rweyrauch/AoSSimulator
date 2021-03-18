/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <sylvaneth/SylvanethBase.h>
#include <Weapon.h>

namespace Sylvaneth {

    class KurnothHunters : public SylvanethBase {
    public:

        enum WeaponOption {
            Greatswords,
            Scythes,
            Greatbows
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        KurnothHunters();

        ~KurnothHunters() override = default;

        bool configure(int numModels, WeaponOption weapons);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onEndCombat(PlayerId player) override;

        WeaponOption m_weaponOption = Greatswords;

    private:

        Weapon m_greatbow,
                m_greatbowHuntmaster,
                m_greatsword,
                m_greatswordHuntmaster,
                m_scythe,
                m_scytheHuntmaster,
                m_viciousClaws;

        bool m_usingTanglethornThicket = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Tanglethorn Thicket              TODO
// Envoys of the Everqueen          TODO
// Trample Underfoot                Yes
// Sundering Strike                 Yes
//

} // namespace Sylvaneth
