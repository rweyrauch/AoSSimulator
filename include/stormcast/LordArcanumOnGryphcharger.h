/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDARCANUMONGRYPHCHARGER_H
#define LORDARCANUMONGRYPHCHARGER_H

#include <stormcast/StormcastEternals.h>
#include <stormcast/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class LordArcanumOnGryphcharger : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static void Init();

        static int ComputePoints(int numModels);

        LordArcanumOnGryphcharger();

        ~LordArcanumOnGryphcharger() override = default;

        bool configure(Lore lore, MountTrait trait);

    protected:

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onStartCombat(PlayerId player) override;

        void onRestore() override { m_shatteredFlasks = false; }

    private:

        bool m_shatteredFlasks = false;

        Weapon m_aetherstave,
                m_beakAndClaws;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aethereal Strike                 Yes
// Ride the Winds Aetheric          TODO
// Spirit Flask                     Yes
// Healing Light                    Yes
// Prime Electrids                  Yes
// Soul Energy                      TODO

} // namespace StormcastEternals

#endif //LORDARCANUMONGRYPHCHARGER_H