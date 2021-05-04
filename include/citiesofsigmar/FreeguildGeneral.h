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

    class FreeguildGeneral : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        FreeguildGeneral(City city, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~FreeguildGeneral() override;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int inspiringLeader(const Unit *target);

    private:

        Weapon m_zweihander;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Decapitating Strike              Yes
// Inspiring Leader                 Yes
// Hold the Line                    TODO
//

} // namespace CitiesOfSigmar
