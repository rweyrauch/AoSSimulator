/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef STEGADON_H
#define STEGADON_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class Stegadon : public SeraphonBase
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 0;

    enum WeaponOption
    {
        SkystreakBow,
        SunfireThrowers
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; }
    static void Init();

    Stegadon();
    ~Stegadon() override = default;

    bool configure(WeaponOption option, bool skinkChief);

    int move() const override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;
    void onCharged() override;

private:

    Weapon m_javelins,
        m_bow,
        m_throwers,
        m_warspear,
        m_horns,
        m_jaws,
        m_stomps;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Armoured Crest                   No
// Gout of Sunfire                  No
// Steadfast Majesty                No
// Unstoppable Stampede             Yes
// Skink Chief                      No
// Coordinated Strike               No
//

} // namespace Seraphon

#endif //STEGADON_H