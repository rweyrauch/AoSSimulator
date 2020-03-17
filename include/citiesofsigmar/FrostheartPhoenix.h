/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FROSTHEARTPHOENIX_H
#define FROSTHEARTPHOENIX_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FrostheartPhoenix : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 220;
    static const int POINTS_PER_UNIT_WITH_ANOINTED = 320;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    FrostheartPhoenix();
    ~FrostheartPhoenix() override;

    bool configure(bool anointed);

protected:

    void onWounded() override;
    void onRestore() override;

    int blizzardAura(const Unit* attacker, const Weapon* weapon, const Unit* target);

private:

    int getDamageTableIndex() const;

    Weapon m_talons,
        m_halberd;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Attuned to Magic                 TODO
// Blizzard Aura                    Yes
// Witness to Destiny               TODO
// Captain of the Phoenix Guard     TODO
//

} // namespace CitiesOfSigmar

#endif //FROSTHEARTPHOENIX_H