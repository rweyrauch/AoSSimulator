/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KAIRICACOLYTES_H
#define KAIRICACOLYTES_H

#include <Unit.h>
#include <Weapon.h>

namespace Tzeentch
{

class KairicAcolytes : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    enum WeaponOptions
    {
        CursedBlade,
        PairedCursedBlades,
        CursedBladeAndShield
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    KairicAcolytes();
    ~KairicAcolytes() override = default;

    bool configure(int numModels, WeaponOptions weapons, int numCursedGlaives, int numScrollsOfDarkArts, int numVulcharcs);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_sorcerousBolt,
        m_sorcerousBoldAdept,
        m_cursedBlade,
        m_cursedGlaive;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Arcanite Shields                 No
// Gestalt Sorcery                  No
// Paired Cursed Blades             No
// Scroll of Dark Arts              No
// Vulcharc                         No
//

} // Tzeentch

#endif //KAIRICACOLYTES_H