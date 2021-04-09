/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <tzeentch/TzeentchBase.h>
#include <Weapon.h>

namespace Tzeentch {

    class HorrorsOfTzeentch : public TzeentchBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        HorrorsOfTzeentch(ChangeCoven coven, int numModels, bool iconBearer, bool hornblower);

        HorrorsOfTzeentch() = delete;

        ~HorrorsOfTzeentch() override = default;

    protected:

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

        void restoreModels(int numModels) override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int castingModifier() const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit *attackingUnit) override;

        Rerolls hornblower(const Unit *unit);

    private:

        lsignal::slot m_hornblowerSlot;

        Weapon  m_magicalFlamesPink{Weapon::Type::Missile, "Magical Flames (Pink)", 12, 3, 5, 4, 0, 1},
                m_magicalFlamesBlue{Weapon::Type::Missile, "Magical Flames (Blue)", 12, 2, 5, 4, 0, 1},
                m_magicalFlamesBrimstone{Weapon::Type::Missile, "Magical Flames (Brimstone)", 12, 1, 5, 4, 0, 1},
                m_talonedHandsPink{Weapon::Type::Melee, "Taloned Hands (Pink)", 1, 1, 5, 4, 0, 1},
                m_talonedHandsBlue{Weapon::Type::Melee, "Taloned Hands (Blue)", 1, 1, 5, 4, 0, 1},
                m_talonedHandsBrimstone{Weapon::Type::Melee, "Taloned Hands (Brimstone)", 1, 2, 5, 4, 0, 1},
                m_talonedHandsIridescent{Weapon::Type::Melee, "Taloned Hands (Iridescent)", 1, 2, 5, 4, 0, 1};

        static bool s_registered;
    };

//Pink Horrors
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearer                      Yes
// Hornblower                       Yes
// Flickering Flames                Yes
// Locus of Conjuration             Yes
// Ectoplasmic Elasticity           Yes
// Split and Split Again            TODO
// Petty Vengeance                  TODO
// Channelled Pink Fire             TODO
//

} // Tzeentch
