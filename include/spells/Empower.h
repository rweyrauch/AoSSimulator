/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef EMPOWER_H
#define EMPOWER_H

#include <Spell.h>

class Empower : public Spell {
public:
    explicit Empower(Unit *caster);

    Result cast(Unit *target, int round) override;

    Result cast(double x, double y, int round) override { return Failed; }

protected:

};

#endif// EMPOWER_H