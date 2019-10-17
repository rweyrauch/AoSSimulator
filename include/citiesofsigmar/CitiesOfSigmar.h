/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CITIESOFSIGMAR_H
#define CITIESOFSIGMAR_H

#include <Unit.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class CitizenOfSigmar : public Unit
{
public:
    CitizenOfSigmar() = default;
    ~CitizenOfSigmar() override = default;

    enum City
    {
        Hammerhal,
        LivingCity,
        GreywaterFastness,
        Phoenicium,
        Anvilgard,
        Hallowheart,
        TempestsEye
    };

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    void setCity(City city);

protected:
    CitizenOfSigmar(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

protected:

protected:

    City m_city = Hammerhal;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
//

void Init();

} // namespace CitiesOfSigmar

#endif // CITIESOFSIGMAR_H