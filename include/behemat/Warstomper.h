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

    class Warstomper : public SonsOfBehematBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        Warstomper();

        Warstomper(CommandTrait trait, Artefact artefact, FierceLoathing loathing, bool isGeneral);

        ~Warstomper() override = default;

    protected:

        size_t getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int terror(const Unit *unit);

        void onBeginRound(int battleRound) override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

    private:
        Weapon m_grip{Weapon::Type::Melee, "Death Grip", 3, 1, 3, 2, -3, RAND_D6},
               m_jump{Weapon::Type::Melee, "Jump Up and Down", 3, 4, 3, 3, -2, RAND_D3},
               m_club{Weapon::Type::Melee, "Titanic Boulderclub", 3, 0, 3, 3, -2, 2};

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Almighty Jump                    Yes
// Crushing Charge                  Yes
// Death Grip                       Yes
// Hurled Body                      TODO
// Longshanks                       TODO
// Sons of Behemat                  TODO
// Terror                           Yes
// Timberrrrr!                      TODO
// Titanic Boulderclub              TODO
// Artefacts
//   Ironweld Cestus                Partial/TODO
//   Club of the First Oak          TODO
//   Mantle of the Destroyer        TODO
//

} // namespace SonsOfBehemat
