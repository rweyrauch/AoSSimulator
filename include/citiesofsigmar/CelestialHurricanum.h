/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HURRICANUM_H
#define HURRICANUM_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class CelestialHurricanum : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 11;
    static const int POINTS_PER_UNIT = 220;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    CelestialHurricanum();
    ~CelestialHurricanum() override = default;

    int move() const override;

    bool configure(bool battlemage);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onWounded() override;

private:

    int getDamageTableIndex() const;

    Weapon m_stormOfShemtek,
        m_wizardStaff,
        m_arcaneTools,
        m_hooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Celestial Battlemage             No
// Locus of Azyr                    No
// Portents of Battle               No
// Storm of Shemtek                 No
// Chain Lightning                  No
// Comet of Casandora               No
//

} // namespace CitiesOfSigmar

#endif //HURRICANUM_H