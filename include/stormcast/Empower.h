/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <Spell.h>

class Empower : public Spell {
public:
    explicit Empower(Unit *caster);

protected:

    Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override;

    Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                 double y) override { return Result::Failed; }

};
