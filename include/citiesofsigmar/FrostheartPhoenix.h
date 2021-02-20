/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class FrostheartPhoenix : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        FrostheartPhoenix();

        ~FrostheartPhoenix() override;

        bool configure(bool anointed);

    protected:

        void onWounded() override;

        void onRestore() override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        int blizzardAura(const Unit *attacker, const Weapon *weapon, const Unit *target);

        int woundModifier() const override;

    private:

        int getDamageTableIndex() const;

        Weapon m_talons,
                m_halberd;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Attuned to Magic                 TODO
// Blizzard Aura                    Yes
// Witness to Destiny               Yes
// Captain of the Phoenix Guard     TODO
//

} // namespace CitiesOfSigmar
