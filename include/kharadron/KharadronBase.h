/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHARADRONBASE_H
#define KHARADRONBASE_H

#include <Unit.h>
#include <Weapon.h>

namespace KharadronOverlords
{

class KharadronBase : public Unit
{
public:
    enum Skyport
    {
        None,
        BarakNar,
        BarakZilfin,
        BarakZon,
        BarakUrbaz,
        BarakMhornar,
        BarakThryng,
        Custom
    };

    enum Artycle
    {
        HonourIsEverything,
        MasterTheSkies,
        SettleTheGrudges
    };

    enum Amendment
    {
        AlwaysTakeWhatYouAreOwed,
        ProsecuteWarsWithAllHaste,
        TrustToYourGuns
    };

    enum Footnote
    {
        TheresNoRewardWithoutRisk,
        TheresNoTradingWithSomePeople,
        WithoutOurShipsWeAreNaught
    };

    KharadronBase() = default;
    ~KharadronBase() override = default;

    static std::string ValueToString(const Parameter& parameter);

    static int EnumStringToInt(const std::string& enumString);

    void setSkyport(Skyport skyport);
    void setCode(Artycle artycle, Amendment amendment, Footnote footnote);

protected:
    KharadronBase(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

protected:

    Skyport m_skyport = None;

    // Code for Custom Skyport
    Artycle m_artycle = HonourIsEverything;
    Amendment m_amendment = AlwaysTakeWhatYouAreOwed;
    Footnote m_footnote = TheresNoRewardWithoutRisk;
};

//
// Abilities                    Implemented
// -------------------------------------------
//

void Init();

} // namespace KharadronOverlords

#endif // KHARADRONBASE_H