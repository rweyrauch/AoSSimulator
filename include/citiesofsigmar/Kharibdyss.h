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

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
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
// Abilities                    Implemented
// -------------------------------------------
// Abyssal Howl                     Yes
// Feast of Bones                   TODO
// Quick With The Lash              TODO
//

} // namespace CitiesOfSigmar

#endif //KHARIBDYSS_H