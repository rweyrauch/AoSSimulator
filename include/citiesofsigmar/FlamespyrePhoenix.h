/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FLAMESPYREPHOENIX_H
#define FLAMESPYREPHOENIX_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class FlamespyrePhoenix : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 200;
    static const int POINTS_PER_UNIT_WITH_ANOINTED = 300;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    FlamespyrePhoenix();
    ~FlamespyrePhoenix() override = default;

    bool configure(bool anointed);

protected:

    void onWounded() override;
    void onRestore() override;

    Wounds applyWoundSave(const Wounds &wounds) override;

private:

    int getDamageTableIndex() const;

    Weapon m_talons,
        m_halberd;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Attuned to Magic                 TODO
// Phoenix Reborn                   TODO
// Wake of Fire                     TODO
// Witness to Destiny               Yes
// Captain of the Phoenix Guard     TODO
//

} // namespace CitiesOfSigmar

#endif //FLAMESPYREPHOENIX_H