/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARHAMMERSIM_CELESTARBALLISTA_H
#define WARHAMMERSIM_CELESTARBALLISTA_H

#include <Unit.h>
#include <Weapon.h>

namespace StormcastEternals
{

class CelestarBallista : public Unit
{
public:

    static const int BASESIZE = 60;
    static const int WOUNDS = 7;

    CelestarBallista();
    ~CelestarBallista() override = default;

    bool configure();

    bool hasKeyword(const std::string& word) const override;

protected:

private:

    static Weapon s_stormboltsSingle,
        s_stormboltsRapid,
        s_sigmariteBlades;

    static std::vector<std::string> s_keywords;
};

} // namespace StormcastEternals

#endif //WARHAMMERSIM_CELESTARBALLISTA_H