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

    class Kharibdyss : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        Kharibdyss();

        ~Kharibdyss() override;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        int abyssalHowl(const Unit *target);

        void onEndCombat(PlayerId player) override;

        int rollChargeDistance() override;

    private:

        size_t getDamageTableIndex() const;

        bool useQuickWithTheLash() const;

        Weapon m_tentacles,
                m_tail,
                m_limbs,
                m_goadsAndWhips;

        lsignal::slot m_connection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Abyssal Howl                     Yes
// Feast of Bones                   Yes
// Quick With The Lash              Yes
//

} // namespace CitiesOfSigmar
