/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKELETONWARRIORS_H
#define SKELETONWARRIORS_H

#include <death/LegionOfNagash.h>
#include <Weapon.h>

namespace Death
{

class SkeletonWarriors : public LegionOfNagashBase
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
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    SkeletonWarriors();
    ~SkeletonWarriors() override = default;

    bool configure(int numModels, WeaponOptions weapons, bool standardBearers, bool hornblowers);

protected:

    int toHitModifier(const Weapon* weapon, const Unit* target) const override;
    int toSaveModifier(const Weapon* weapon) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    bool m_standardBearers = false;
    bool m_hornblowers = false;

    Weapon m_ancientBlade,
        m_ancientBladeChampion,
        m_ancientSpear,
        m_ancientSpearChampion;

    static bool s_registered;

};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Standard Bearer                  No
// Hornblower                       No
// Skeleton Legion                  Yes
// Crypt Shields                    Yes
//

} //namespace Death

#endif //SKELETONWARRIORS_H