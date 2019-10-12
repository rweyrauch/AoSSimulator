/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Think.h>
#include <Unit.h>
#include <Spell.h>
#include <Prayer.h>
#include <CommandAbility.h>
#include <Board.h>

Spell* SelectSpell(Unit* caster, AbilityTarget& outTarget)
{
    outTarget.targetUnit = false;
    outTarget.target = nullptr;
    outTarget.x = 0.0f;
    outTarget.y = 0.0f;

    for (auto sip = caster->spellBegin(); sip != caster->spellEnd(); ++sip)
    {
        if (*sip == nullptr) continue;

        auto units = Board::Instance()->getUnitsWithin(caster, GetEnemyId(caster->owningPlayer()), (*sip)->range());
        if ((*sip)->targetFriendly())
        {
            units = Board::Instance()->getUnitsWithin(caster, caster->owningPlayer(), (*sip)->range());
            units.push_back(caster);
        }

        // TODO: determine 'best' spell to cast on the 'best' target.

        // Compute 'score' for using spell, sip, to target units.
    }

    return nullptr;
}

Prayer* SelectPrayer(Unit* priest, AbilityTarget& outTarget)
{
    outTarget.targetUnit = false;
    outTarget.target = nullptr;
    outTarget.x = 0.0f;
    outTarget.y = 0.0f;

    for (auto pip = priest->prayerBegin(); pip != priest->prayerEnd(); ++pip)
    {
        if (*pip == nullptr) continue;

        auto units = Board::Instance()->getUnitsWithin(priest, GetEnemyId(priest->owningPlayer()), (*pip)->range());
        if ((*pip)->targetFriendly())
        {
            units = Board::Instance()->getUnitsWithin(priest, priest->owningPlayer(), (*pip)->range());
            units.push_back(priest);
        }

        // Compute 'score' for using prayer, pip, to target units.
    }

    return nullptr;
}

CommandAbility* SelectCommandAbility(Unit* commander, AbilityTarget& outTarget)
{
    outTarget.targetUnit = false;
    outTarget.target = nullptr;
    outTarget.x = 0.0f;
    outTarget.y = 0.0f;

    for (auto cip = commander->commandAbilityBegin(); cip != commander->commandAbilityEnd(); ++cip)
    {
        if (*cip == nullptr) continue;

        auto units = Board::Instance()->getUnitsWithin(commander, GetEnemyId(commander->owningPlayer()), (*cip)->range());
        if ((*cip)->targetFriendly())
        {
            units = Board::Instance()->getUnitsWithin(commander, commander->owningPlayer(), (*cip)->range());
            units.push_back(commander);
        }

        // Compute 'score' for using command ability, cip, to target units.
    }

    return nullptr;
}