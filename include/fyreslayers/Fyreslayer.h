/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FYRESLAYER_H
#define FYRESLAYER_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <map>

namespace Fyreslayers {

    enum Lodge {
        Custom = 0,
        Vostarg,
        Greyfyrd,
        Hermdar,
        Lofnir
    };

    enum class Rune {
        OfFury,
        OfSearingHeat,
        OfAwakenedSteel,
        OfFieryDetermination,
        OfRelentlessZeal,
        OfFarsight
    };

    const std::array<int, 5> g_lodge = {
            Custom,
            Vostarg,
            Greyfyrd,
            Hermdar,
            Lofnir
    };

    class Fyreslayer : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        Fyreslayer() = default;

        ~Fyreslayer() override = default;

        void setLodge(Lodge lodge);

    protected:
        Fyreslayer(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void onEndRound(int battleRound) override;

        void onStartHero(PlayerId player) override;

        int chargeModifier() const override;

        int rollRunDistance() const override;

        void activateRune();

    protected:

        Lodge m_lodge = Custom;

        bool m_activatedRune = false;
        std::map<Rune, bool> m_availableRunes;

    };

    void Init();

} // namespace Fyreslayers

#endif //FYRESLAYER_H