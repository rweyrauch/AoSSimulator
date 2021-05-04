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

    class Battlemage : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Battlemage(City city, Realm realm, Lore lore, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~Battlemage() override = default;

    protected:

        int castingModifier() const override;

    private:

        Weapon m_staff;
        Realm m_realm = Realm::None;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Magic of the Realms              Yes
// Chain Lightning                  Yes
// Fireball                         Yes
// Mystifying Miasma                Yes
// Pall of Doom                     Yes
// Pha's Protection                 Yes
// Shield of Thorns                 TODO
// Transmutation of Lead            Yes
// Wildform                         Yes
//

} // namespace CitiesOfSigmar
