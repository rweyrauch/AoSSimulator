/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/ContortedEpitome.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include <slaanesh/Lore.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {

    class OverwhelmingAcquiescence : public Spell {
    public:
        explicit OverwhelmingAcquiescence(Unit *caster);

    protected:

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue,
                     Unit *target) override { return Result::Failed; }

        Result
        apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override;
    };

    OverwhelmingAcquiescence::OverwhelmingAcquiescence(Unit *caster) :
            Spell(caster, "Overwhelming Acquiescence", 7, 24) {
        m_allowedTargets = Abilities::Target::Point;
        m_effect = Abilities::EffectType::Debuff;
    }

    Spell::Result
    OverwhelmingAcquiescence::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                                    double y) {

        auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), m_range);

        auto numUnits = Dice::RollD3();
        int numAffected = 0;

        for (auto unit : units) {
            unit->buffReroll(Attribute::Target_To_Hit_Missile, Rerolls::Ones, defaultDuration());
            unit->buffReroll(Attribute::Target_To_Hit_Melee, Rerolls::Ones, defaultDuration());

            numAffected++;
            if (numAffected > numUnits) break;
        }
        return Result::Success;
    }

    static const int g_basesize = 25;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 210;

    bool TheContortedEpitome::s_registered = false;

    TheContortedEpitome::TheContortedEpitome() :
            SlaaneshBase("The Contorted Epitome", 12, g_wounds, 10, 5, false),
            m_ravagingClaws(Weapon::Type::Melee, "Ravaging Claws", 1, 9, 3, 4, -1, 1),
            m_coiledTentacles(Weapon::Type::Melee, "Coiled Tentacles", 3, 2, 3, 4, -2, 2) {
        m_keywords = {CHAOS, DAEMON, DAEMONETTE, SLAANESH, HEDONITE, HERO, WIZARD, HERALD_OF_SLAANESH,
                      THE_CONTORTED_EPITOME};
        m_weapons = {&m_ravagingClaws, &m_coiledTentacles};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    void TheContortedEpitome::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_ravagingClaws);
        model->addMeleeWeapon(&m_coiledTentacles);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<OverwhelmingAcquiescence>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    Unit *TheContortedEpitome::Create(const ParameterList &parameters) {
        auto unit = new TheContortedEpitome();

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);

        unit->configure(lore);
        return unit;
    }

    void TheContortedEpitome::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheContortedEpitome::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    TheContortedEpitome::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("The Contorted Epitome", factoryMethod);
        }
    }

    Wounds TheContortedEpitome::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Swallow Energy
        Dice::RollResult mortalSaves;
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.mortal -= mortalSaves.rollsGE(2);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int TheContortedEpitome::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
