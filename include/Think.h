/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef THINK_H
#define THINK_H

class Unit;

class Spell;

class Prayer;

class CommandAbility;

struct AbilityTarget {
    bool targetUnit = false;
    Unit *target = nullptr;
    double x = 0, y = 0;
};

Spell *SelectSpell(Unit *caster, AbilityTarget &outTarget);

Prayer *SelectPrayer(Unit *priest, AbilityTarget &outTarget);

CommandAbility *SelectCommandAbility(Unit *commander, AbilityTarget &outTarget);

#endif //THINK_H