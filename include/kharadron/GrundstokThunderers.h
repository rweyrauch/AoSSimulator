/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GRUNDSTOKTHUNDERERS_H
#define GRUNDSTOKTHUNDERERS_H

#include <kharadron/KharadronBase.h>
#include <Weapon.h>

namespace KharadronOverlords {

    class GrundstokThunderers : public KharadronBase {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        GrundstokThunderers();

        ~GrundstokThunderers() override = default;

        bool configure(int numModels, int numMortars, int numCannons, int numFumigators, int numDecksweeper,
                       int numHonourBearers);

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

#endif //GRUNDSTOKTHUNDERERS_H
