/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BATTLEMAGE_H
#define BATTLEMAGE_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class Battlemage : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Battlemage();
    ~Battlemage() override = default;

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
// Magic of the Realms              No
// Chain Lightning                  No
// Fireball                         No
// Mystifying Miasma                No
// Pall of Doom                     No
// Pha's Protection                 No
// Shield of Thorns                 No
// Transmutation of Lead            No
// Wildform                         No
//

} // namespace CitiesOfSigmar

#endif //BATTLEMAGE_H