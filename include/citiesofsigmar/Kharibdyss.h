/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHARIBDYSS_H
#define KHARIBDYSS_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar
{

class Kharibdyss : public CitizenOfSigmar
{
public:

    static const int BASESIZE = 105;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 170;

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels) { return POINTS_PER_UNIT; };
    static void Init();

    Kharibdyss();
    ~Kharibdyss() override;

    bool configure();

protected:

    void onWounded() override;
    void onRestore() override;
    int abyssalHowl(const Unit* target);

private:

    int getDamageTableIndex() const;

    Weapon m_tentacles,
        m_tail,
        m_limbs,
        m_goadsAndWhips;

    lsignal::slot m_connection;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Abyssal Howl                     Yes
// Feast of Bones                   No
// Quick With The Lash              No
//

} // namespace CitiesOfSigmar

#endif //KHARIBDYSS_H