/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class Endrinriggers : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Endrinriggers(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, int numModels, int numVolleyGuns,
                      int numDrills, int numGrapnels, int numSkyhooks, int points);

        ~Endrinriggers() override = default;

    protected:

        Wounds weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const override;

        int chargeModifier() const override;

        void onStartHero(PlayerId player) override;

    private:

        Weapon m_volleyGun,
                m_skyhook,
                m_drillLauncher,
                m_rivetGun,
                m_saw,
                m_gunButt,
                m_sawMaster,
                m_gunButtMaster;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Drill Launcher                   Yes
// Endrincraft                      Yes
// Grapnel Launcher                 TODO
// Hitchers                         TODO
// Skyhook                          Yes
//

} // namespace KharadronOverlords

