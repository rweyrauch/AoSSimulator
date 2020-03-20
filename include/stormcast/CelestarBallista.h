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

    static Unit *Create(const ParameterList &parameters);
    static int ComputePoints(int numModels);
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
// Abilities                    Implemented
// -------------------------------------------
// Bastions of Death                TODO
// Chained Lightning                Yes
// Versatile Weapon                 Yes
//

} // namespace StormcastEternals

#endif //CELESTARBALLISTA_H