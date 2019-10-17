/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEGENERALGRIFFON_H
#define FREEGENERALGRIFFON_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FreeguildGeneralOnGriffon : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 13;
    static const int POINTS_PER_UNIT = 320;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    FreeguildGeneralOnGriffon();
    ~FreeguildGeneralOnGriffon() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:


    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Charging Lance                   No
// Freeguild Shield                 No
// Skilled Rider                    No
// Piercing Bloodroar               No
// Rousing Battle Cry               No
//

} // namespace CitiesOfSigmar

#endif //FREEGENERALGRIFFON_H