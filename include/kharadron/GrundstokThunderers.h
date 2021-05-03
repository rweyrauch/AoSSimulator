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

    class GrundstokThunderers : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        GrundstokThunderers(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, int numModels, int numMortars,
                            int numCannons, int numFumigators, int numDecksweeper, int numHonourBearers, int points);

        ~GrundstokThunderers() override = default;

    protected:

    private:

        Weapon m_rifle,
                m_doubleBarrelledRifle,
                m_fumigator,
                m_decksweeper,
                m_aethercannon,
                m_mortar,
                m_drillbill,
                m_gunButt;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Honour Bearer                    TODO
// Chocking Fug                     TODO
// Drive Them Back!                 TODO
// Pin THem, Shred Them, Finish Them    TODO
//

} // namespace KharadronOverlords

