/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#pragma once

#include <Spell.h>

class MysticShield : public Spell {
public:
    explicit MysticShield(Unit *caster);

    Result cast(Unit *target, int round) override;

    Result cast(double x, double y, int round) override { return Result::Failed; }

protected:

};
