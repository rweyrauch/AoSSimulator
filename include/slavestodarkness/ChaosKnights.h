/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <slavestodarkness/SlavesToDarkness.h>
#include <Weapon.h>

namespace SlavesToDarkness {

    class ChaosKnights : public SlavesToDarknessBase {
    public:

        enum WeaponOption {
            Ensorcelled_Weapon = 0,
            Cursed_Lance,
            Cursed_Flail,    // Doom Knight only
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        ChaosKnights(DamnedLegion legion, MarkOfChaos mark, int numModels, WeaponOption weapons, WeaponOption doomKnightWeapon, 
                     bool standardBearer, bool hornblower, int points);

        ChaosKnights() = delete;
        
        ~ChaosKnights() override;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        int runModifier() const override;

        int chargeModifier() const override;

        int braveryModifier() const override;

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int terrifyingChampions(const Unit *unit);

    private:

        Weapon  m_ensorcelledWeapon{Weapon::Type::Melee, "Ensorcelled Weapon", 1, 3, 3, 3, -1, 1},
                m_lance{Weapon::Type::Melee, "Cursed Lance", 2, 2, 4, 3, 0, 1},
                m_ensorcelledWeaponLeader{Weapon::Type::Melee, "Ensorcelled Weapon", 1, 4, 3, 3, -1, 1},
                m_lanceLeader{Weapon::Type::Melee, "Cursed Lance", 2, 3, 4, 3, 0, 1},
                m_flailLeader{Weapon::Type::Melee, "Cursed Flail", 2, RAND_D6, 4, 3, 0, 1},
                m_hooves{Weapon::Type::Melee, "Trampling Hooves", 1, 2, 4, 4, 0, 1};

        lsignal::slot m_terrifyingSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Hornblower                       Yes
// Chaos Runeshields                Yes
// Mark of Chaos                    Yes
// Impaling Charge                  Yes
// Terrifying Champions             Yes
//

} // SlavesToDarkness
