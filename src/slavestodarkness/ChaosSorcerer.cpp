/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "slavestodarkness/ChaosSorcerer.h"
#include "SlavesToDarknessPrivate.h"
#include "StDSpells.h"

namespace SlavesToDarkness {

    class DaemonicPower : public Spell {
    public:
        explicit DaemonicPower(Unit *caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override;

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    DaemonicPower::DaemonicPower(Unit *caster) :
            Spell(caster, "Daemonic Power", 6, 18) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords.push_back(MORTAL);
        m_targetKeywords.push_back(SLAVES_TO_DARKNESS);
    }

    Spell::Result
    DaemonicPower::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        target->buffReroll(Attribute::To_Hit_Melee, Rerolls::Failed, defaultDuration());
        target->buffReroll(Attribute::To_Hit_Missile, Rerolls::Failed, defaultDuration());
        target->buffReroll(Attribute::To_Wound_Melee, Rerolls::Failed, defaultDuration());
        target->buffReroll(Attribute::To_Wound_Missile, Rerolls::Failed, defaultDuration());

        return Spell::Result::Success;
    }

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 110;

    bool ChaosSorcerer::s_registered = false;

    Unit *ChaosSorcerer::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new ChaosSorcerer(legion, mark, lore, trait, artefact, general);
    }

    void ChaosSorcerer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosSorcerer::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosSorcerer::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Sorcerer Lord", factoryMethod);
        }
    }

    ChaosSorcerer::ChaosSorcerer(DamnedLegion legion, MarkOfChaos mark, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlavesToDarknessBase("Chaos Sorcerer Lord", 5, g_wounds, 7, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, WIZARD,
                      CHAOS_SORCERER_LORD};
        m_weapons = {&m_staff, &m_blade};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setDamnedLegion(legion);
        setMarkOfChaos(mark);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<DaemonicPower>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    int ChaosSorcerer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void ChaosSorcerer::onEndHero(PlayerId player) {
        SlavesToDarknessBase::onEndHero(player);

        // Oracular Visions
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit : units) {
                if ((unit->remainingModels() > 0) && unit->hasKeyword(SLAVES_TO_DARKNESS) && unit->hasKeyword(MORTAL)) {
                    unit->buffReroll(Attribute::To_Save_Melee, Rerolls::Failed, {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                    unit->buffReroll(Attribute::To_Save_Missile, Rerolls::Failed, {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                }
            }
        }
    }

} //namespace SlavesToDarkness
