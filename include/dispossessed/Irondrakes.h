/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed {

    class Irondrakes : public Dispossessed {
    public:

        enum WeaponOptions {
            Drakegun,
            Grudgehammer_Torpedo,
            Drakefire_Pistol_And_Cinderblast_Bomb,
            Paired_Drakefire_Pistols
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Irondrakes(int points);

        ~Irondrakes() override = default;

        bool configure(int numModels, WeaponOptions ironWardenWeapons, bool iconBearer, bool hornblower);

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override;

        void onStartShooting(PlayerId player) override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int rollRunDistance() override;

    private:

        bool m_hasCinderblastBomb = false;

        Weapon m_drakegun,
                m_drakegunWarden,
                m_grudgehammerTorpedo,
                m_drakefirePistol,
                m_drakefirePistolMelee,
                m_mailedFist;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brace of Drakefire Pistols       Yes
// Grudgehammer Torpedo             Yes
// Cinderblast Bomb                 Yes
// Forge-proven Gromril Armour      Yes
// Blaze Away                       Yes
// Hornblowers                      Yes
// Icon Bearer                      TODO
//

} // namespace Dispossessed
