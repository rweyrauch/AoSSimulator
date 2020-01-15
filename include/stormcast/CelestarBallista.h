/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CELESTARBALLISTA_H
#define CELESTARBALLISTA_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>


namespace StormcastEternals
{

class CelestarBallista : public StormcastEternal
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 110;

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels)  { return POINTS_PER_UNIT; }
    static void Init();

    CelestarBallista();

    ~CelestarBallista() override = default;

    bool configure();

protected:

    void onStartShooting(PlayerId player) override;

private:

    Weapon m_stormboltsSingle,
        m_stormboltsRapid,
        m_sigmariteBlades;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Bastions of Death                No
// Chained Lightning                Yes
// Versatile Weapon                 Yes
//

} // namespace StormcastEternals

#endif //CELESTARBALLISTA_H