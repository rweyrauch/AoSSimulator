/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHYDRA_H
#define WARHYDRA_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class WarHydra : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 170;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    WarHydra();
    ~WarHydra() override = default;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;

private:

    int getDamageTableIndex() const;

    Weapon m_fieryBreath,
        m_fangs,
        m_limbs,
        m_goadAndWhips;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Quick with the Lash              TODO
// Sever One Head, Another
//    Takes Its Place               TODO
//

} // namespace CitiesOfSigmar

#endif //WARHYDRA_H