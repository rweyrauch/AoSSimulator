/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKELETONWARRIORS_H
#define SKELETONWARRIORS_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{

class SkeletonWarriors : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    enum WeaponOptions
    {
        AncientBlade,
        AncientSpear,
    };

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    SkeletonWarriors();
    ~SkeletonWarriors() override = default;

    bool configure(int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    bool m_standardBearers = false;
    bool m_hornblowers = false;

    static Weapon s_ancientBlade,
        s_ancientBladeChampion,
        s_ancientSpear,
        s_ancientSpearChampion;

    static bool s_registered;

};

} //namespace Death

#endif //SKELETONWARRIORS_H