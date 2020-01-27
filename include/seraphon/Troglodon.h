/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TROGLODON_H
#define TROGLODON_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class Troglodon : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 0;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    Troglodon();
    ~Troglodon() override = default;

    bool configure();

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_spittle,
        m_bite,
        m_forelimbs,
        m_rod;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Divining Rod                     No
// Primal Roar                      No
// Drawn to the Screams             No
//

} // namespace Seraphon

#endif //TROGLODON_H