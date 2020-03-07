/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SKINKS_H
#define SKINKS_H

#include <seraphon/Seraphon.h>
#include <Weapon.h>

namespace Seraphon
{

class Skinks : public SeraphonBase
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    enum WeaponOption
    {
        JavelinsDaggersAndBucklers,
        BoltspittersAndClubs,
        BoltspittersDaggersAndBucklers,
        ClubsAndBucklers
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Skinks();
    ~Skinks() override = default;

    bool configure(int numModels, WeaponOption weapons);

protected:

    int toSaveModifier(const Weapon *weapon) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    WeaponOption m_weaponOption = BoltspittersDaggersAndBucklers;

    Weapon m_javelin,
        m_boltspitter,
        m_dagger,
        m_daggerAlpha,
        m_club,
        m_clubAlpha;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Star-buckler                     Yes
// Swarming Cohort                  Yes
//

} // namespace Seraphon

#endif //SKINKS_H