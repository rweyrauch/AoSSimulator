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
    class SavageOrruks : public Bonesplitterz {
    public:

        enum WeaponOption {
            Chompa,
            Savage_Stikka,
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int ComputePoints(const ParameterList& parameters);

        static int EnumStringToInt(const std::string &enumString);

        SavageOrruks(Warclan warclan, int numModels, WeaponOption weapons, bool skullThumper, bool totemBearer, int points);

        ~SavageOrruks() override = default;

    protected:

        int chargeModifier() const override {
            int mod = Bonesplitterz::chargeModifier();
            if (isNamedModelAlive("Skull Thumper")) {
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

        int toSaveModifier(const Weapon *weapon, const Unit* attacker) const override {
            // Bone Shield
            int mod = Unit::toSaveModifier(weapon, attacker);
            if (!weapon->isMissile()) {
                mod++;
            }
            return mod;
        }

    private:

        Weapon m_chompa,
                m_stikka,
                m_chompaBoss,
                m_stikkaBoss;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Spirit of Gorkamorka             Yes
// Bone Shield                      Yes
// Thumper                          Yes
// Totem Bearer                     Yes
//

} // namespace Bonesplitterz
