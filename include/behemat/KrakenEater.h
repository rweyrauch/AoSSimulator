/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <behemat/SonsOfBehemat.h>

namespace SonsOfBehemat {

    class KrakenEater : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        KrakenEater();

        KrakenEater(CommandTrait trait, Artefact artefact, FierceLoathing loathing, bool isGeneral);

        ~KrakenEater() override = default;

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int terror(const Unit *unit);

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:
        Weapon m_debris{Weapon::Type::Missile, "Hurled Debris", 24, 3, 4, 3, -1, RAND_D3},
               m_stomp{Weapon::Type::Melee, "Death Grip", 3, 1, 3, 2, -3, RAND_D6},
               m_grip{Weapon::Type::Melee, "Death Grip", 3, 1, 3, 2, -3, RAND_D6},
               m_warclub{Weapon::Type::Melee, "Shipwrecka Warclub", 3, 8, 3, 3, -2, 2};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Almighty Stomp                   Yes
// Crushing Charge                  Yes
// Death Grip                       Yes
// Longshanks                       TODO
// Get Orf Me Land!                 TODO
// Stuff 'em in Me Net              TODO
// Son of Behemat                   TODO
// Terror                           Yes
// Timberrrrr!                      TODO
// Artefacts
//   Jaws of the Mogalodon          TODO
//   Wallopin' Tentacle             TODO
//   Jar of Burny Grog              TODO
//   Net of the Beast-reaver        TODO
//   Glowy Lantern                  Yes
//   Krakenskin Sandals             TODO
//

} // namespace SonsOfBehemat
