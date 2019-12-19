/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef PLAGUEMONKS_H
#define PLAGUEMONKS_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven
{

class PlagueMonks : public Skaventide
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
        PairedFoetidBlades,
        FoetidBladeAndWoeStave
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    PlagueMonks();
    ~PlagueMonks() override = default;

    bool configure(int numModels, WeaponOptions weapons, int contagionBanners, int iconsOfPestilence, int doomGongs, int baleChimes);

    void visitWeapons(std::function<void(const Weapon &)> &visitor) override;

protected:

    void onStartHero(PlayerId player) override;
    Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;
    int runModifier() const override;
    int chargeModifier() const override;
    Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;
    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

private:

    WeaponOptions m_weapons = PairedFoetidBlades;
    int m_numContagionBanners = 0;
    int m_numIconsOfPestilence = 0;
    int m_numDoomGongs = 0;
    int m_numBaleChimes = 0;

    Weapon m_foetidBlade,
        m_woeStave;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Contagion Banner                 No
// Icon of Pestilence               Yes
// Doom Gong                        Yes
// Bale-chime                       No
// Pair of Foetid Blades            Yes
// Frenzied Assault                 Yes
// Book of Woes                     Yes

} // namespace Skaven

#endif //PLAGUEMONKS_H