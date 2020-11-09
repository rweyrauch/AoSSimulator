/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LUMINARKOFHYSH_H
#define LUMINARKOFHYSH_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class LuminarkOfHysh : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        LuminarkOfHysh();

        ~LuminarkOfHysh() override;

        bool configure(bool battlemage, Lore lore);

    protected:

        void onWounded() override;

        void onRestore() override;

        int castingModifier() const override;

        int locusOfHysh(const Unit *caster);

    private:

        int getDamageTableIndex() const;

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
// Burning Gaze                     TODO
// Pha's Protection                 TODO
//

} // namespace CitiesOfSigmar

#endif //LUMINARKOFHYSH_H