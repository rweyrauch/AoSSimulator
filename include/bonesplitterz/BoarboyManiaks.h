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

    class SavageBoarboyManiaks : public Bonesplitterz {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static int ComputePoints(const ParameterList& parameters);

        SavageBoarboyManiaks(Warclan warclan, int numModels, bool boarThumper, bool totemBearer, int points);

        ~SavageBoarboyManiaks() override = default;

    protected:

        int chargeModifier() const override {
            int mod = Bonesplitterz::chargeModifier();
            if (isNamedModelAlive("Boar Thumper")) {
                mod += 2;
            }
            return mod;
        }

        int braveryModifier() const override {
            int mod = Bonesplitterz::braveryModifier();
            if (isNamedModelAlive("Totem Bearer")) {
                mod++;
            }
            return mod;
        }

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int toWoundModifier(const Weapon *weapon, const Unit *target) const override;

    private:

        Weapon  m_chompas{Weapon::Type::Melee, "Pair of Chompas", 1, 4, 4, 3, 0, 1},
                m_tusksAndHooves{Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1},
                m_chompasBoss{Weapon::Type::Melee, "Pair of Chompas", 1, 5, 4, 3, 0, 1};

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          Yes
// Totem Bearer                     Yes
// Maniak Fury                      Yes
// Tusker Charge                    Yes
//

} // namespace Bonesplitterz
