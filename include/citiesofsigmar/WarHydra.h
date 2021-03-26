/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class WarHydra : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        WarHydra();

        ~WarHydra() override = default;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        void onStartHero(PlayerId player) override;

        int rollChargeDistance() override;

    private:

        size_t getDamageTableIndex() const;

        bool useQuickWithTheLash() const;

        Weapon m_fieryBreath,
                m_fangs,
                m_limbs,
                m_goadAndWhips;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Quick with the Lash              Yes
// Sever One Head, Another
//    Takes Its Place               Yes
//

} // namespace CitiesOfSigmar
