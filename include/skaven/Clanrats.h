/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CLANRATS_H
#define CLANRATS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class Clanrats : public Skaventide
{
public:
    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 20;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 200;

    enum WeaponOptions
    {
        RustySpear,
        RustyBlade
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Clanrats();
    ~Clanrats() override = default;

    bool configure(int numModels, WeaponOptions weapons, bool clanshields, int standardBearers, int bellRingers);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int toSaveModifier(const Weapon *weapon) const override;
    int runModifier() const override;

private:

    bool m_clanshields = false;
    int m_numStandardBearers = 0;
    int m_numBellRingers = 0;

    Weapon m_rustySpear,
        m_rustySpearLeader,
        m_rustyBlade,
        m_rustyBladeLeader;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  Yes
// Bell-ringer                      Yes
// Clanshields                      Yes

} // namespace Skaven

#endif //CLANRATS_H