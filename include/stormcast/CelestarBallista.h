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
    static const int POINTS_PER_UNIT = 100;

    static Unit *Create(const ParameterList &parameters);

    static void Init();

    CelestarBallista();

    ~CelestarBallista() override = default;

    void shooting(PlayerId player) override;

    bool configure();

    void visitWeapons(std::function<void(const Weapon *)> &visitor) override;

protected:

private:

    Weapon m_stormboltsSingle,
        m_stormboltsRapid,
        m_sigmariteBlades;

    static bool s_registered;
};

} // namespace StormcastEternals

#endif //CELESTARBALLISTA_H