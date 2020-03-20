/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef ORRUKMORBOYS_H
#define ORRUKMORBOYS_H

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz {
    class SavageOrrukMorboys : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        SavageOrrukMorboys();

        ~SavageOrrukMorboys() override = default;

        bool configure(int numModels, bool skullThumper, bool totemBearer);

    protected:

        int chargeModifier() const override {
            int mod = Unit::chargeModifier();
            if (m_thumper) {
                mod += 2;
            }
            return mod;
        }

        int braveryModifier() const override {
            int mod = Unit::braveryModifier();
            if (m_totemBearer) {
                mod++;
            }
            return mod;
        }

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

    private:

        bool m_thumper = false;
        bool m_totemBearer = false;

        Weapon m_chompaAndShiv,
                m_chompaAndShivBoss;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          Yes
// Totem Bearer                     Yes
// Spirit of Gorkamorka             Yes
// Power of the Beast Spirit        TODO
//

} // namespace Bonesplitterz

#endif //ORRUKMORBOYS_H