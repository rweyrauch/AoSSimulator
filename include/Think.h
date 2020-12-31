/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

class Unit;

class Spell;

class Prayer;

class CommandAbility;

struct AbilityTarget {
    bool m_targetUnit = false;
    Unit *m_target = nullptr;
    double m_x = 0, m_y = 0;
};

Spell *SelectSpell(Unit *caster, AbilityTarget &outTarget);

Prayer *SelectPrayer(Unit *priest, AbilityTarget &outTarget);

CommandAbility *SelectCommandAbility(Unit *commander, AbilityTarget &outTarget);
