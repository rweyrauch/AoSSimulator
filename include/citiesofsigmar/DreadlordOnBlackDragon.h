/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DREADLORDBLACKDRAGON_H
#define DREADLORDBLACKDRAGON_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class DreadlordOnBlackDragon : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 300;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    DreadlordOnBlackDragon();
    ~DreadlordOnBlackDragon() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:


private:

    Weapon m_crossbow,
        m_noxiousBreath,
        m_blade,
        m_lance,
        m_jaws,
        m_claws;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Lance of Spite                   No
// Noxious Breath                   No
// Paired Exile Blades              No
// Tyrant Shield                    No
// Do Not Disappoint Me             No
//

} // namespace CitiesOfSigmar

#endif //DREADLORDBLACKDRAGON_H