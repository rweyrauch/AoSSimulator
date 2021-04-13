/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include "bonesplitterz/Bonesplitterz.h"

namespace Bonesplitterz {

    class SavageOrrukArrowboys : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(int numModels);

        SavageOrrukArrowboys();

        ~SavageOrrukArrowboys() override = default;

    protected:

        bool configure(int numModels, bool skullThumper, bool totemBearer);

        int chargeModifier() const override {
            int mod = Unit::chargeModifier();
            if (isNamedModelAlive("Skull Thumper")) {
                mod += 2;
            }
            return mod;
        }

        int braveryModifier() const override {
            int mod = Unit::braveryModifier();
            if (isNamedModelAlive("Totem Bearer")) {
                mod++;
            }
            return mod;
        }

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

    private:

        Weapon m_stingaBow,
                m_boneShiv,
                m_chompa;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          Yes
// Totem Bearer                     Yes
// Aim Fer Its Eyes                 Yes
// Loadsa Arrows                    Yes
//

} // namespace Bonesplitterz
