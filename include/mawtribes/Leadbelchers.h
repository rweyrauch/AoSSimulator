/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LEADBELCHERS_H
#define LEADBELCHERS_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class Leadbelchers : public MawtribesBase
{
public:

    static const int BASESIZE = 40;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Leadbelchers();
    ~Leadbelchers() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onStartShooting(PlayerId player) override;

private:

    Weapon m_gun,
        m_blow,
        m_bite,
        m_blowThunderfist;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Thunderous Blasts of Hot Metal   Yes
//

} // namespace OgorMawtribes

#endif //LEADBELCHERS_H
