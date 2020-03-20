/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef BADMOON_H
#define BADMOON_H

namespace GloomspiteGitz {

    class BadMoon {
    public:
        static BadMoon *Instance();

        enum class Location {
            Rising,     // Rising not yet on the board.
            Northwest,
            Northeast,
            Southwest,
            Southeast,
            All,        // In center of board, affecting all quadrants
            Set         // Moon has set and no longer affects the board.
        };

        bool setup(Location initialLocation);

        bool move(int round);

        Location location() const;

    private:

        BadMoon() = default;

        ~BadMoon() = default;

        void advanceOne();

    private:

        int m_round = 0;
        int m_position[2] = {0, 0};

        Location m_initialLocation = Location::Rising; // Determine direction of motion
        Location m_location = Location::Rising;

        static BadMoon *s_instance;
    };

} //namespace GloomspiteGitz

#endif //BADMOON_H
