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

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Battlemage();
    ~Battlemage() override = default;

    bool configure(Realm realm);

protected:

    int castingModifier() const override;

private:

    Weapon m_staff;
    Realm m_realm = Azyr;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Magic of the Realms              Yes
// Chain Lightning                  TODO
// Fireball                         TODO
// Mystifying Miasma                TODO
// Pall of Doom                     TODO
// Pha's Protection                 TODO
// Shield of Thorns                 TODO
// Transmutation of Lead            TODO
// Wildform                         TODO
//

} // namespace CitiesOfSigmar

#endif //BATTLEMAGE_H