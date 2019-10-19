/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FREEOUTRIDERS_H
#define FREEOUTRIDERS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FreeguildOutriders : public CitizenOfSigmar
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
        Blunderbuss,
        BraceOfPistols
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    FreeguildOutriders();
    ~FreeguildOutriders() override = default;

    bool configure(int numModels, bool trumpeter, WeaponOption sharpshooterWeapon);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:


private:

    bool m_trumpeter = false;

    Weapon m_blunderbuss,
        m_pistols,
        m_handgun,
        m_sabre,
        m_hooves,
        m_sabreSharpshooter;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Trumpeter                        No
// Expert Gunners                   No
// Skilled Riders                   No
//

} // namespace CitiesOfSigmar

#endif //FREEHANDGUNNER_H