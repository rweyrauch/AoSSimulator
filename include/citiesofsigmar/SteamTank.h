/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STEAMTANK_H
#define STEAMTANK_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class SteamTank : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 200;
    static const int POINTS_FOR_COMMANDER = 50;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    SteamTank();
    ~SteamTank() override = default;

    bool configure(bool commander);

protected:

    void onWounded() override;
    void onRestore() override;

private:

    int getDamageTableIndex() const;

    Weapon m_steamCannon,
        m_steamGun,
        m_longRifle,
        m_handgun,
        m_crushingWheels,
        m_sword;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Bouncing Cannon Ball             No
// I'll Fix It                      No
// More Pressure!                   No
// Steel Behemoth                   No
// Target Sighted                   No
//

} // namespace CitiesOfSigmar

#endif //STEAMTANK_H