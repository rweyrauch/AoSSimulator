/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef CELESTIALHURRICANUM_H
#define CELESTIALHURRICANUM_H

#include <citiesofsigmar/CitiesOfSigmar.h>
#include <Weapon.h>

namespace CitiesOfSigmar {

    class CelestialHurricanum : public CitizenOfSigmar {
    public:

        static Unit *Create(const ParameterList &parameters);

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        static void Init();

        CelestialHurricanum();

        ~CelestialHurricanum() override;

        bool configure(bool battlemage, Lore lore);

    protected:

        void onWounded() override;

        void onRestore() override;

        void onStartShooting(PlayerId player) override;

        int castingModifier() const override;

        int locusOfAzyr(const Unit *caster);

        int portentsOfBattle(const Unit *attacker, const Weapon *weapon, const Unit *unit);

    private:

        int getDamageTableIndex() const;

        Weapon m_stormOfShemtek,
                m_wizardStaff,
                m_arcaneTools,
                m_hooves;

        lsignal::slot m_locusConnection;
        lsignal::slot m_portentsConnection;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Celestial Battlemage             Yes
// Locus of Azyr                    Yes
// Portents of Battle               Yes
// Storm of Shemtek                 Yes
// Chain Lightning                  TODO
// Comet of Casandora               TODO
//

} // namespace CitiesOfSigmar

#endif //CELESTIALHURRICANUM_H