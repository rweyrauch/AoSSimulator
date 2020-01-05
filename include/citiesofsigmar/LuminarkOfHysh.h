/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LUMINARK_H
#define LUMINARK_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class LuminarkOfHysh : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 11;
    static const int POINTS_PER_UNIT = 210;
    static const int POINTS_PER_UNIT_WITH_BATTLEMAGE = 270;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    LuminarkOfHysh();
    ~LuminarkOfHysh() override = default;

    int move() const override;

    bool configure(bool battlemage);

protected:

    void onWounded() override;
    int castingModifier() const override;

private:

    int getDamageTableIndex() const;

    Weapon m_beamOfLight,
        m_wizardsStaff,
        m_arcaneTools,
        m_hooves;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Aura of Protection               No
// Locus of Hysh                    No
// Searing Beam of Light            No
// White Battlemage                 Yes
// Burning Gaze                     No
// Pha's Protection                 No
//

} // namespace CitiesOfSigmar

#endif //LUMINARK_H