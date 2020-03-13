/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VOKMORTIAN_H
#define VOKMORTIAN_H

#include <ossiarch/OssiarchBonereaperBase.h>
#include <Weapon.h>

namespace OssiarchBonereapers
{

class Vokmortian : public OssiarchBonereaperBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 180;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Vokmortian();
    ~Vokmortian() override;

    bool configure();

protected:

    int grimWarning(const Unit* unit);

private:

    Weapon m_gazeOfDeath,
        m_staff;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Contract of Nagash               TODO
// Grim Warning                     Partial/TODO
// Mortal Touch                     TODO
//

} // namespace OssiarchBonereapers

#endif //VOKMORTIAN_H
