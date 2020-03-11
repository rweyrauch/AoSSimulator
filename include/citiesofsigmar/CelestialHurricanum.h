/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HURRICANUM_H
#define HURRICANUM_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class CelestialHurricanum : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 11;
    static const int POINTS_PER_UNIT = 220;
    static const int POINTS_PER_UNIT_WITH_BATTLEMAGE = 280;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    CelestialHurricanum();
    ~CelestialHurricanum() override;

    bool configure(bool battlemage);

protected:

    void onWounded() override;
    void onRestore() override;
    void onStartShooting(PlayerId player) override;
    int castingModifier() const override;

    int locusOfAzyr(const Unit* caster);
    int portentsOfBattle(const Weapon* weapon, const Unit* unit);

private:

    int getDamageTableIndex() const;

    Weapon m_stormOfShemtek,
        m_wizardStaff,
        m_arcaneTools,
        m_hooves;

    lsignal::slot m_locusConnection;
    lsignal::slot m_portentsConnection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Celestial Battlemage             Yes
// Locus of Azyr                    Yes
// Portents of Battle               Yes
// Storm of Shemtek                 Yes
// Chain Lightning                  No
// Comet of Casandora               No
//

} // namespace CitiesOfSigmar

#endif //HURRICANUM_H