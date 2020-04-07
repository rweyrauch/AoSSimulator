/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DRAKESWORNTEMPLAR_H
#define DRAKESWORNTEMPLAR_H

#include <stormcast/StormcastEternals.h>
#include <Weapon.h>

namespace StormcastEternals {

    class DrakeswornTemplar : public StormcastEternal {
    public:

        enum WeaponOption {
            TempestAxe,
            ArcHammer,
            Stormlance,
        };

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        DrakeswornTemplar();

        ~DrakeswornTemplar() override;

        bool configure(WeaponOption weapons, bool skyboltBow);

    protected:

        void onWounded() override;

        int getDamageTableIndex() const;

        void onStartShooting(PlayerId player) override;

        void onStartCombat(PlayerId player) override;

        Wounds onEndCombat(PlayerId player) override;

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Wounds weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        void onRestore() override;

        int arcaneLineage(const Unit *target);

    protected:

        WeaponOption m_weaponOption = TempestAxe;

    private:

        Weapon m_skyboltBow,
                m_tempestAxe,
                m_arcHammer,
                m_stormlance,
                m_greatClaws;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Arcane Lineage                   Yes
// Cavernous Jaws                   Yes
// Sweeping Tail                    Yes
// Arc Hammer                       Yes
// Lord of the Heavens              Yes
// Skybolt Bow                      TODO
// Stormlance                       Yes
// Tempest Axe                      TODO
//

} // namespace StormcastEternals

#endif //DRAKESWORNTEMPLAR_H