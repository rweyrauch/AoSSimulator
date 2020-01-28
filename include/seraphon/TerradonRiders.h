/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TERRADONS_H
#define TERRADONS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class TerradonRiders : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 9;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    enum WeaponOption
    {
        StarstrikeJavelins,
        SunleechBolas
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    TerradonRiders();
    ~TerradonRiders() override = default;

    bool configure(int numModels, WeaponOption option, bool masterOfTheSkies);

protected:

private:

    Weapon m_javelin,
        m_javelinLeader,
        m_bolas,
        m_bolasLeader,
        m_beak,
        m_skyblade;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Deadly Cargo                     No
// Sunleech Bolas                   No
// Swooping Dive                    No
// Skyblade                         No
//

} // namespace Seraphon

#endif //TERRADONS_H