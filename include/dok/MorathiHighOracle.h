/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MORATHIORACLE_H
#define MORATHIORACLE_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class MorathiHighOracleOfKhaine : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        MorathiHighOracleOfKhaine();

        ~MorathiHighOracleOfKhaine() override = default;

        bool configure();

    protected:

        int targetHitModifier(const Weapon *weapon, const Unit *attacker) const override;

        int castingModifier() const override;

        int unbindingModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds) override;

    private:

        int m_woundsInTurn = 0;

        Weapon m_heartrender,
                m_wings;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Monstrous Transformation         TODO
// The Truth Revealed               TODO
// The Iron Heart of Khaine         TODO
// Sorceress Supreme                Yes
// Enchanting Beauty                Yes
// Arnzipal's Black Horror          TODO
// Worship Through Bloodshed        TODO
//

} // namespace DaughtersOfKhaine

#endif //MORATHIORACLE_H