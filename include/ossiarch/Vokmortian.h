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

    static const int BASESIZE = 0;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Vokmortian();
    ~Vokmortian() override = default;

    bool configure();

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

private:


    Weapon m_gazeOfDeath,
        m_staff;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Contract of Nagash               No
// Grim Warning                     No
// Mortal Touch                     No
//

} // namespace OssiarchBonereapers

#endif //VOKMORTIAN_H
