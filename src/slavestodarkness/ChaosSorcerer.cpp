/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "slavestodarkness/ChaosSorcerer.h"
#include "SlavesToDarknessPrivate.h"
#include "StDSpells.h"

namespace SlavesToDarkness {

    class DaemonicPower : public Spell {
    public:
        explicit DaemonicPower(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override  { return Result::Failed; }
    };

    DaemonicPower::DaemonicPower(Unit *caster) :
            Spell(caster, "Daemonic Power", 6, 18) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords.push_back(MORTAL);
        m_targetKeywords.push_back(SLAVES_TO_DARKNESS);
    }

    Spell::Result DaemonicPower::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) {
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
        auto unit = new ChaosSorcerer();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    ChaosSorcerer::ChaosSorcerer() :
            SlavesToDarknessBase("Chaos Sorcerer Lord", 5, g_wounds, 7, 4, false),
            m_staff(Weapon::Type::Melee, "Sorcerous Staff", 2, 1, 4, 3, -1, RAND_D3),
            m_blade(Weapon::Type::Melee, "Chaos Runeblade", 1, 2, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, WIZARD,
                      CHAOS_SORCERER_LORD};
        m_weapons = {&m_staff, &m_blade};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool ChaosSorcerer::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<DaemonicPower>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int ChaosSorcerer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace SlavesToDarkness
