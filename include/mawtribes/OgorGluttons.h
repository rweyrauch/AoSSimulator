/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef OGORGLUTTONS_H
#define OGORGLUTTONS_H

#include <mawtribes/MawtribesBase.h>
#include <Weapon.h>

namespace OgorMawtribes {

    class OgorGluttons : public MawtribesBase {
    public:

        enum WeaponOption {
            Club_Or_Blade_And_Ironfist,
            Paired_Club_Or_Blade
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        OgorGluttons();

        ~OgorGluttons() override;

        bool configure(int numModels, WeaponOption option, bool skullBearer, bool bannerBearer, bool lookoutGnoblar,
                       bool bellower);

    protected:

        int braveryModifier() const override;

        Rerolls chargeRerolls() const override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Wounds computeReturnedDamage(const Weapon *weapon, int saveRoll) const override;

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        int bellower(const Unit *target);

    private:

        WeaponOption m_weaponOption = Club_Or_Blade_And_Ironfist;

        bool m_bellower = false;
        bool m_skullBearer = false;
        bool m_bannerBearer = false;
        bool m_lookoutGnoblar = false;

        Weapon m_clubOrBlade,
                m_bite,
                m_clubOrBladeCrusher;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bellower                         Yes
// Beast Skull Bearer               Yes
// Tribal Banner Bearer             Yes
// Lookout Gnoblar                  Yes
// Paired Clubs or Blades           Yes
// Ironfist                         Yes

} // namespace OgorMawtribes

#endif //OGORGLUTTONS_H
