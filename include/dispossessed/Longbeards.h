/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LONGBEARDS_H
#define LONGBEARDS_H

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed {

    class Longbeards : public Dispossessed {
    public:

        enum WeaponOptions {
            Ancestral_Axes_Or_Hammers,
            Ancestral_Great_Axe
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        Longbeards();

        ~Longbeards() override = default;

        bool configure(int numModels, WeaponOptions weapons, bool gromrilShields, bool standardBearer, bool musician);

    protected:

        Rerolls toSaveRerolls(const Weapon *weapon) const override;

        int rollRunDistance() const override;

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

    private:

        bool m_gromrilShields = false;
        bool m_standardBearer = false;
        bool m_musician = false;

        Weapon m_ancestralAxeHammer,
                m_ancestralGreatAxe,
                m_ancestralAxeHammerOldGuard,
                m_ancestralGreatAxeOldGuard;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Gromril Shields                  Yes
// Old Grumblers                    TODO
// Standard Bearer                  Yes
// Musician                         Yes
//

} // namespace Dispossessed

#endif //LONGBEARDS_H