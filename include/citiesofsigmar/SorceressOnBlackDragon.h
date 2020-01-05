/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SORCERESSBLACKDRAGON_H
#define SORCERESSBLACKDRAGON_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class SorceressOnBlackDragon : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    SorceressOnBlackDragon();
    ~SorceressOnBlackDragon() override = default;

    int move() const override;

    bool configure();

protected:

    void onWounded() override;

private:

    int getDamageTableIndex() const;

    Weapon m_noxiousBreath,
        m_rod,
        m_sword,
        m_lash,
        m_jaws,
        m_claws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Blood Sacrifice                  No
// Noxious Breath                   No
// Bladewind                        No
// Command Underlings               No
// Inspire Hatred                   No
//

} // namespace CitiesOfSigmar

#endif //SORCERESSBLACKDRAGON_H