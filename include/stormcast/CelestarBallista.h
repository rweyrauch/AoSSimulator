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

    CelestarBallista();
    ~CelestarBallista() override = default;

    bool configure(bool singleShot);

protected:

private:

    static Weapon s_stormboltsSingle,
        s_stormboltsRapid,
        s_sigmariteBlades;
};

} // namespace StormcastEternals

#endif //CELESTARBALLISTA_H