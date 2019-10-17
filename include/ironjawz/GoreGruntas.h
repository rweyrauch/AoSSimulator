/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GOREGRUNTAS_H
#define GOREGRUNTAS_H

#include <ironjawz/Ironjawz.h>

namespace Ironjawz
{

class OrrukGoreGruntas : public Ironjawz
{
public:
    static const int BASESIZE = 90;  // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    enum WeaponOption
    {
        PigIronChoppa = 0,
        JaggedGorehacka,
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter& parameter);

    static int EnumStringToInt(const std::string& enumString);

    static void Init();

    OrrukGoreGruntas();
    ~OrrukGoreGruntas() override = default;

    bool configure(int numModels, WeaponOption weapons);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

protected:

    Weapon m_pigironChoppa,
        m_jaggedGorehacka,
        m_tusksAndHooves,
        m_bossPigironChoppa,
        m_bossJaggedGorehacka;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Gore-grunta Charge               No
//

} // namespace Ironjawz

#endif //GOREGRUNTAS_H