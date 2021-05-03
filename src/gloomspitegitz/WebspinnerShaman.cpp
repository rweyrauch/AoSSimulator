/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/WebspinnerShaman.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>
#include <Board.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {

    class SpeedOfTheSpiderGod : public Spell {
    public:
        explicit SpeedOfTheSpiderGod(Unit *caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override;

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Spell::Result::Failed; }

    };

    SpeedOfTheSpiderGod::SpeedOfTheSpiderGod(Unit *caster) :
            Spell(caster, "Speed of the Spider God", 4, 24) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords.push_back(SPIDERFANG);
    }

    Spell::Result
    SpeedOfTheSpiderGod::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        target->buffMovement(MovementRule::Run_And_Shoot, true, defaultDuration());

        if (castingValue >= 8) {
            // Pick D3 additional units
            auto additionalUnits = Dice::RollD3() - 1; // skip unit that was already buffed
            auto units = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), m_range);
            for (auto unit : units) {
                if (!unit->hasKeyword(SPIDERFANG)) continue;

                unit->buffMovement(MovementRule::Run_And_Shoot, true, defaultDuration());

                additionalUnits--;
                if (additionalUnits <= 0) continue;
            }
        }
        return Spell::Result::Success;
    }

    static const int g_basesize = 32;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 80;

    bool WebspinnerShaman::s_registered = false;

    WebspinnerShaman::WebspinnerShaman(Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            GloomspiteGitzBase("Webspinner Shaman", 5, g_wounds, 4, 6, false, g_pointsPerUnit),
            m_spiderGodStaff(Weapon::Type::Melee, "Spider God Staff", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, SPIDERFANG, HERO, WIZARD, WEBSPINNER_SHAMAN};
        m_weapons = {&m_spiderGodStaff};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_spiderGodStaff);

        m_knownSpells.push_back(std::make_unique<SpeedOfTheSpiderGod>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        addModel(model);
    }

    Unit *WebspinnerShaman::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters, g_loreOfTheSpiderFangs[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_giftsOfTheGloomspite[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_venomousValuables[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new WebspinnerShaman(lore, trait, artefact, general);
    }

    void WebspinnerShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WebspinnerShaman::Create,
                    WebspinnerShaman::ValueToString,
                    WebspinnerShaman::EnumStringToInt,
                    WebspinnerShaman::ComputePoints,
                    {
                            EnumParameter("Lore of the Spiderfangs", g_loreOfTheSpiderFangs[0], g_loreOfTheSpiderFangs),
                            EnumParameter("Command Trait", g_giftsOfTheGloomspite[0], g_giftsOfTheGloomspite),
                            EnumParameter("Artefact", g_venomousValuables[0], g_venomousValuables),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Webspinner Shaman", factoryMethod);
        }
    }

    std::string WebspinnerShaman::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Spiderfangs") {
            return ToString((Lore) parameter.intValue);
        }
        return GloomspiteGitzBase::ValueToString(parameter);
    }

    int WebspinnerShaman::EnumStringToInt(const std::string &enumString) {
        return GloomspiteGitzBase::EnumStringToInt(enumString);
    }

    int WebspinnerShaman::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds WebspinnerShaman::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Touched by the Spider God
        return ignoreWounds(wounds, 5);
    }

} // namespace GloomspiteGitz
