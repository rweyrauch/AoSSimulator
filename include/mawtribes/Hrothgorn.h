/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HROTHGORN_H
#define HROTHGORN_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Hrothgorn : public MawtribesBase
{
public:

    static Unit* Create(const ParameterList& parameters);
    static void Init();
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);

    Hrothgorn();
    ~Hrothgorn() override = default;

    bool configure();

protected:

private:

    Weapon m_trapLauncher,
        m_knife,
        m_bite;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Masters of Ambush                TODO
// Thrafnir                         TODO
//

} // namespace OgorMawtribes

#endif //HROTHGORN_H