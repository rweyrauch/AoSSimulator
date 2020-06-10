/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <spells/GloomspiteSpells.h>
#include <Unit.h>
#include <Board.h>

namespace GloomspiteGitz {

    std::string ToString(Lore which) {
        switch (which) {
            case None:
                return "None";
            case VindictiveGlare:
                return "Vindictive Glare";
            case ItchyNuisance:
                return "Itchy Nuisance";
            case TheGreatGreenSpite:
                return "The Great Green Spite";
            case TheHandOfGork:
                return "The Hand of Gork";
            case SquigLure:
                return "Squig Lure";
            case CallDaMoon:
                return "Call da Moon";
            case DeadlyWebbing:
                return "Deadly Webbing";
            case VenomousSpiderlings:
                return "Venomous Spiderlings";
            case ScuttlingTerrors:
                return "Scuttling Terrors";
            case SneakyDistraction:
                return "Sneaky Distraction";
            case CurseOfDaSpiderGod:
                return "Curse of da Spider God";
            case GiftOfDaSpiderGod:
                return "Gift of da Spider God";
        }
        return "";
    }

    bool FromString(const std::string &enumString, Lore &outLore) {
        bool valid = true;

        if (enumString == "None")
            outLore = None;
        else if (enumString == "Vindictive Glare")
            outLore = VindictiveGlare;
        else if (enumString == "Itchy Nuisance")
            outLore = ItchyNuisance;
        else if (enumString == "The Great Green Spite")
            outLore = TheGreatGreenSpite;
        else if (enumString == "The Hand of Gork")
            outLore = TheHandOfGork;
        else if (enumString == "Squig Lure")
            outLore = SquigLure;
        else if (enumString == "Call da Moon")
            outLore = CallDaMoon;
        else if (enumString == "Deadly Webbing")
            outLore = DeadlyWebbing;
        else if (enumString == "Venomous Spiderlings")
            outLore = VenomousSpiderlings;
        else if (enumString == "Scuttling Terrors")
            outLore = ScuttlingTerrors;
        else if (enumString == "Sneaky Distraction")
            outLore = SneakyDistraction;
        else if (enumString == "Curse of da Spider God")
            outLore = CurseOfDaSpiderGod;
        else if (enumString == "Gift of da Spider God")
            outLore = GiftOfDaSpiderGod;
        else
            valid = false;
        return valid;
    }

    DamageSpell *CreateVindictiveGlare(Unit *caster) {
        return new DamageSpell(caster, "Vindictive Glare", 5, 12, RAND_D3);
    }

    class GreatGreenSpite : public Spell {
    public:
        explicit GreatGreenSpite(Unit *caster);

        Result cast(Unit *target, int round) override;

        Result cast(double x, double y, int round) override { return Failed; }
    };

    GreatGreenSpite::GreatGreenSpite(Unit *caster) :
            Spell(caster, "The Great Green Spite", 7, 18.0) {}

    Spell::Result GreatGreenSpite::cast(Unit *target, int round) {
        if (target == nullptr) {
            return Failed;
        }

        // Distance to target
        const double distance = m_caster->distanceTo(target);
        if (distance > m_range) {
            return Failed;
        }

        auto unit = Board::Instance()->getNearestUnit(m_caster, GetEnemyId(m_caster->owningPlayer()));

        // Check for visibility to enemy unit
        if ((unit == nullptr) || !Board::Instance()->isVisible(m_caster, unit)) {
            return Failed;
        }

        Spell::Result result = Failed;

        int damage = 1;
        if (target->remainingModels() >= 20)
            damage = RAND_D6;
        else if (target->remainingModels() >= 10)
            damage = RAND_D3;

        int mortalWounds = 0;
        const int castingRoll = m_caster->rollCasting();
        if (castingRoll >= m_castingValue) {
            bool unbound = Board::Instance()->unbindAttempt(m_caster, castingRoll);
            if (!unbound) {
                mortalWounds = Dice::rollSpecial(damage);
                target->applyDamage({0, mortalWounds, Wounds::Source::Spell});
                SimLog(Verbosity::Narrative,
                       "%s spell %s with casting roll of %d (%d) inflicts %d mortal wounds into %s.\n",
                       m_caster->name().c_str(), name().c_str(), castingRoll, m_castingValue, mortalWounds,
                       target->name().c_str());
                result = Success;
            } else {
                result = Unbound;
            }
        }

        return result;
    }

    Spell *CreateLore(Lore which, Unit *caster) {
        switch (which) {
            case None:
                return nullptr;
            case VindictiveGlare:
                return CreateVindictiveGlare(caster);
            case ItchyNuisance:
                return nullptr;
            case TheGreatGreenSpite:
                return new GreatGreenSpite(caster);
            case TheHandOfGork:
                return nullptr;
            case SquigLure:
                return nullptr;
            case CallDaMoon:
                return nullptr;
            case DeadlyWebbing:
                return nullptr;
            case VenomousSpiderlings:
                return nullptr;
            case ScuttlingTerrors:
                return nullptr;
            case SneakyDistraction:
                return nullptr;
            case CurseOfDaSpiderGod:
                return nullptr;
            case GiftOfDaSpiderGod:
                return nullptr;
        }
        return nullptr;
    }

} // namespace GloomspiteGitz