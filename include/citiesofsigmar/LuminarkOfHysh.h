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

    class LuminarkOfHysh : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        LuminarkOfHysh(City city, bool battlemage, Lore lore, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~LuminarkOfHysh() override;

    protected:

        void onWounded() override;

        void onRestore() override;

        int castingModifier() const override;

        int locusOfHysh(const Unit *caster);

    private:

        size_t getDamageTableIndex() const;

        Weapon m_beamOfLight,
                m_wizardsStaff,
                m_arcaneTools,
                m_hooves;

        lsignal::slot m_locusSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aura of Protection               TODO
// Locus of Hysh                    Yes
// Searing Beam of Light            TODO
// White Battlemage                 Yes
// Burning Gaze                     Yes
// Pha's Protection                 Yes
//

} // namespace CitiesOfSigmar
