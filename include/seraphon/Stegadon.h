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

    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 240;
    static const int POINTS_PER_UNIT_WITH_CHIEF = 270;

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
    ~Stegadon() override;

    bool configure(WeaponOption option, bool skinkChief);

protected:

    void onWounded() override;
    void onRestore() override;
    int getDamageTableIndex() const;
    void onCharged() override;

    Rerolls steadfastMajestyBraveryReroll(const Unit* unit);

private:

    bool m_skinkChief = false;

    Weapon m_javelins,
        m_bow,
        m_throwers,
        m_warspear,
        m_horns,
        m_jaws,
        m_stomps;

    lsignal::slot m_steadfastSlot;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Armoured Crest                   TODO
// Gout of Sunfire                  TODO
// Steadfast Majesty                Yes
// Unstoppable Stampede             Yes
// Skink Chief                      Yes
// Coordinated Strike               TODO
//

} // namespace Seraphon

#endif //STEGADON_H