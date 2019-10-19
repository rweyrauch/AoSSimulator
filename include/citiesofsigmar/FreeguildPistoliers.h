/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEPISTOLIERS_H
#define FREEPISTOLIERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FreeguildPistoliers : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 400;

    enum WeaponOption
    {
        RepeaterHandgun,
        BraceOfPistols
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    FreeguildPistoliers();
    ~FreeguildPistoliers() override = default;

    bool configure(int numModels, bool trumpeter, WeaponOption outriderWeapon);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:


private:

    bool m_trumpeter = false;

    Weapon m_pistols,
        m_handgun,
        m_sabreAndPistolButt,
        m_hooves,
        m_sabreAndPistolButtOutrider;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Trumpeter                        No
// Hail of Bullets                  No
// Reckless Riders                  No
//

} // namespace CitiesOfSigmar

#endif //FREEPISTOLIERS_H