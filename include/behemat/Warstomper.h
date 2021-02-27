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

        ~Warstomper() override = default;

        bool configure();

    protected:

        int getDamageTableIndex() const;

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int terror(const Unit *unit);

        void onBeginRound(int battleRound) override;

        Rerolls toSaveRerolls(const Weapon *weapon, const Unit *attacker) const override;

    private:
        Weapon m_grip,
            m_jump,
            m_club;

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
