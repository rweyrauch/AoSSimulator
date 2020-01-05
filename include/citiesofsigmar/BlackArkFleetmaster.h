/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLACKARKFLEETMASTER_H
#define BLACKARKFLEETMASTER_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class BlackArkFleetmaster : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 60;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    BlackArkFleetmaster();
    ~BlackArkFleetmaster() override = default;

    bool configure();

protected:

    int toSaveModifier(const Weapon *weapon) const override
    {
        // Sea Dragon Cloak
        auto mod = CitizenOfSigmar::toSaveModifier(weapon);
        if (weapon->isMissile()) mod++;
        return mod;
    }

    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

private:

    Weapon m_cutlass,
        m_murderHook;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Murderous Swashbuckler           Yes
// Sea Dragon Cloak                 Yes
// At Them, You Curs!               No
//

} // namespace CitiesOfSigmar

#endif //BLACKARKFLEETMASTER_H