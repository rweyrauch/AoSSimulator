/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VULKITEBERZERKERS_H
#define VULKITEBERZERKERS_H

#include <fyreslayers/Fyreslayer.h>
#include <Weapon.h>

namespace Fyreslayers {

    class VulkiteBerzerkers : public Fyreslayer {
    public:

        enum WeaponOption {
            HandaxeAndShield,
            WarpickAndShield,
            PairedHandaxes
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        VulkiteBerzerkers();

        ~VulkiteBerzerkers() override = default;

        bool configure(int numModels, WeaponOption weapons, bool hornOfGrimnir);

    protected:

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int chargeModifier() const override;

    private:

        WeaponOption m_weaponOption = HandaxeAndShield;
        bool m_hornOfGrimnir = false;

        Weapon m_handaxe,
            m_handaxeKarl,
            m_warpick,
            m_warpickKarl,
            m_throwingAxe;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Horn of Grimnir                  Yes
// Berserk Fury                     TODO
// Fyresteel Handaxes               Yes
// Bladed Slingshield               TODO
//

} // namespace Fyreslayers

#endif //VULKITEBERZERKERS_H