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

    class Troglodon : public SeraphonBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Troglodon(WayOfTheSeraphon way, Constellation constellation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral);

        Troglodon() = delete;

        ~Troglodon() override;

    protected:

        void onWounded() override;

        void onRestore() override;

        size_t getDamageTableIndex() const;

        void onStartHero(PlayerId player) override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int castingModifier() const override;

        int terror(const Unit *target);

    private:

        Weapon  m_spittle{Weapon::Type::Missile, "Noxious Spittle", 18, RAND_D3, 3, 3, 0, 2},
                m_jaws{Weapon::Type::Melee, "Venomous Jaws", 2, 3, 4, 2, 0, 2},
                m_forelimbs{Weapon::Type::Melee, "Clawed Forelimbs", 2, 2, 4, 3, 0, 2},
                m_rod{Weapon::Type::Melee, "Divining Rod", 1, 2, 4, 4, -1, RAND_D3};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Oracle of the Slann              Yes
// Regeneration                     Yes
// Terror                           Yes
// Venomous Spittle                 Yes
// Drawn to the Screams             TODO
// Comet's Call                     Yes
//

} // namespace Seraphon
