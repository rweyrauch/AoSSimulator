/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GHOULKINGTERRORGHEIST_H
#define GHOULKINGTERRORGHEIST_H

#include <fec/FleshEaterCourts.h>
#include <Weapon.h>

namespace FleshEaterCourt {

    class AbhorrantGhoulKingOnTerrorgheist : public FleshEaterCourts {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        AbhorrantGhoulKingOnTerrorgheist();

        ~AbhorrantGhoulKingOnTerrorgheist() override = default;

        bool configure();

        void onSlain() override;

    protected:

        void onStartHero(PlayerId player) override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onWounded() override;

        void onRestore() override;

        int getDamageTableIndex() const;

    private:

        Weapon m_deathShriek,
                m_goryTalonsAndFangs,
                m_skeletalClaws,
                m_fangedMaw;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Death Shriek                     TODO
// Gaping Maw                       Yes
// Infested                         Yes
// Royal Blood                      Yes
// Unholy Vitality                  TODO
// Summon Royal Guard               TODO
//

} // namespace FleshEaterCourt

#endif //GHOULKINGTERRORGHEIST_H