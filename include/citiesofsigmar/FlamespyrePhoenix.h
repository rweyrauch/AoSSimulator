/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FLAMESPYREPHOENIX_H
#define FLAMESPYREPHOENIX_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FlamespyrePhoenix : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 200;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    FlamespyrePhoenix();
    ~FlamespyrePhoenix() override = default;

    bool configure(bool anoited);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_talons,
        m_halberd;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Attuned to Magic                 No
// Phoenix Reborn                   No
// Wake of Fire                     No
// Witness to Destiny               No
// Captain of the Phoenix Guard     No
//

} // namespace CitiesOfSigmar

#endif //FLAMESPYREPHOENIX_H