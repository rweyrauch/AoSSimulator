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

    class Anointed : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Anointed(City city, Lore lore, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~Anointed() override = default;

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

    private:

        Weapon m_halberd;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Blessing of the Ur-Phoenix       Yes
// Witness to Destiny               Yes
// Captain of the Phoenix Guard     TODO
//

} // namespace CitiesOfSigmar
