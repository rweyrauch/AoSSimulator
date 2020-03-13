/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COGSMITH_H
#define COGSMITH_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class Cogsmith : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 60;

    enum WeaponOption
    {
        GrudgeRaker,
        CogAxe
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static int EnumStringToInt(const std::string &enumString);
    static void Init();

    Cogsmith();
    ~Cogsmith() override = default;

    bool configure(WeaponOption weapon);

protected:

    int toHitModifier(const Weapon *weapon, const Unit *target) const override;
    void onStartHero(PlayerId player) override;

private:

    WeaponOption m_weaponOption = GrudgeRaker;

    Weapon m_grudgeRaker,
        m_pistols,
        m_cogAxe,
        m_gunButt;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Free Arm                         Yes
// Master Engineer                  Yes
//

} // namespace CitiesOfSigmar

#endif //COGSMITH_H