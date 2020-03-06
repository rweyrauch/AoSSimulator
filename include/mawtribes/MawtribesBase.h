/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef MAWTRIBESBASE_H
#define MAWTRIBESBASE_H

#include <Unit.h>
#include <Weapon.h>

namespace OgorMawtribes
{

class MawtribesBase : public Unit
{
public:

    enum Mawtribe
    {
        None = 0,
        Meatfist,
        Bloodgullet,
        Underguts,
        Boulderhead,
        Thunderbellies,
        Winterbite
    };

    MawtribesBase() = default;
    ~MawtribesBase() override = default;

    static std::string ValueToString(const Parameter& parameter);
    static int EnumStringToInt(const std::string& enumString);

    void setMawtribe(Mawtribe tribe);

protected:
    MawtribesBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

protected:

    Mawtribe m_tribe = None;

    mutable int m_unmodifiedChargeRoll = 0;

    void onCharged() override;
    void onStartHero(PlayerId player) override;
    int rollChargeDistance() const override;
    int moveModifier() const override;
    int braveryModifier() const override;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Trampling Charge                 Yes
// Grasp of the Everwinter          Yes
// Might Make Right                 No
// Ravenous Brutes                  Yes
// Fleshy Stampede (Meatfist)       Yes
// Heralds of the Gulping God (Bloodgullet) No
// Gunmasters (Underguts)           Yes
// Fearsome Breed (Boulderhead)     No
// Swift Outflank (Thunderbellies)  No
// Ghosts in the Blizzard (Winterbite)  No
//

void Init();

} // namespace OgorMawtribes

#endif // MAWTRIBESBASE_H