/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKULLGRINDER_H
#define SKULLGRINDER_H

#include <khorne/KhorneBase.h>
#include <Weapon.h>

namespace Khorne
{

class Skullgrinder : public KhorneBase
{
public:
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Skullgrinder();
    ~Skullgrinder() override;

    bool configure();

protected:

    int favouredByKhorne(const Unit* unit);

private:

    Weapon m_brazenAnvil;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Fire Anvil                       TODO
// Favoured by Khorne               Yes
//

} // namespace Khorne

#endif //SKULLGRINDER_H