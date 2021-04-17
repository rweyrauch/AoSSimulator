/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/KeeperOfSecrets.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include <slaanesh/Lore.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {

    class CacophonicChoir : public Spell {
    public:
        explicit CacophonicChoir(Unit *caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue,
                     Unit *target) override { return Result::Failed; }

        Result
        apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override;
    };

    CacophonicChoir::CacophonicChoir(Unit *caster) :
            Spell(caster, "Cacophonic Choir", 4, 6) {
        m_allowedTargets = Abilities::Target::Point;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result
    CacophonicChoir::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) {

        const auto roll = Dice::Roll2D6();
        auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), m_range);
        for (auto unit : units) {
            if (unit->bravery() < roll) {
                unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            }
        }
        return Spell::Result::Success;
    }

    static const int g_basesize = 100;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 340;

    struct TableEntry {
        int m_move;
        int m_greatbladeAttacks;
        int m_clawDamage;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 4, 5},
                    {12, 3, 4},
                    {10, 3, 3},
                    {8,  2, 3},
                    {6,  2, 2}
            };

    bool KeeperOfSecrets::s_registered = false;

    KeeperOfSecrets::KeeperOfSecrets() :
            SlaaneshBase("Keeper of Secrets", 14, g_wounds, 10, 4, false),
            m_livingWhip(Weapon::Type::Missile, "Living Whip", 6, 1, 3, 3, -1, 1),
            m_ritualKnifeOrHand(Weapon::Type::Melee, "Ritual Knife or Sinistrous Hand", 1, 1, 2, 3, -1, 1),
            m_greatblade(Weapon::Type::Melee, "Elegant Greatblade", 2, 4, 3, 3, -1, 2),
            m_impalingClaws(Weapon::Type::Melee, "Impaling Claws", 3, 2, 3, 3, -2, 5) {
        m_keywords = {CHAOS, DAEMON, GREATER_DAEMON, SLAANESH, HEDONITE, MONSTER, HERO, WIZARD, KEEPER_OF_SECRETS};
        m_weapons = {&m_livingWhip, &m_ritualKnifeOrHand, &m_greatblade, &m_impalingClaws};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    void KeeperOfSecrets::configure(WeaponOption weapon, Lore lore) {
        auto model = new Model(g_basesize, wounds());

        m_weapon = weapon;

        if (weapon == Living_Whip) {
            model->addMissileWeapon(&m_livingWhip);
        }
        if (weapon == Ritual_Knife || weapon == Sinistrous_Hand) {
            model->addMeleeWeapon(&m_ritualKnifeOrHand);
        }
        model->addMeleeWeapon(&m_greatblade);
        model->addMeleeWeapon(&m_impalingClaws);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<CacophonicChoir>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    Unit *KeeperOfSecrets::Create(const ParameterList &parameters) {
        auto unit = new KeeperOfSecrets();
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Ritual_Knife);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greaterDaemonLore[0]);

        unit->configure(weapon, lore);
        return unit;
    }

    void KeeperOfSecrets::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {Ritual_Knife, Sinistrous_Hand, Living_Whip, Shining_Aegis};
            static FactoryMethod factoryMethod = {
                    KeeperOfSecrets::Create,
                    KeeperOfSecrets::ValueToString,
                    KeeperOfSecrets::EnumStringToInt,
                    KeeperOfSecrets::ComputePoints,
                    {
                            EnumParameter("Weapon", Ritual_Knife, weapons),
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_greaterDaemonLore[0], g_greaterDaemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Keeper of Secrets", factoryMethod);
        }
    }

    void KeeperOfSecrets::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_greatblade.setAttacks(g_damageTable[damageIndex].m_greatbladeAttacks);
        m_impalingClaws.setDamage(g_damageTable[damageIndex].m_clawDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        Unit::onWounded();
    }

    size_t KeeperOfSecrets::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void KeeperOfSecrets::onRestore() {
        SlaaneshBase::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    void KeeperOfSecrets::onEndCombat(PlayerId player) {
        SlaaneshBase::onEndCombat(player);

        auto target = m_meleeTarget;
        if (target == nullptr) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            if (!units.empty()) {
                target = units.front();
            }
        }

        // Ritual Knife
        if ((m_weapon == Ritual_Knife) && target) {
            Wounds knifeWounds = {0, 0};
            int roll = Dice::RollD6();
            if (roll == 6) {
                knifeWounds = {0, Dice::RollD3()};
            } else if (roll >= 2) {
                knifeWounds = {0, 1};
            }
            target->applyDamage(knifeWounds, this);
        }

        // Sinistrous Hand
        if ((m_weapon == Sinistrous_Hand) && m_currentRecord.m_enemyModelsSlain) {
            // TODO: keep track of heroes slain to heal D6
            heal(Dice::RollD3());
        }
    }

    Wounds KeeperOfSecrets::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        if (m_weapon == Shining_Aegis) {
            // Shining Aegis
            return ignoreWounds(wounds, 6);
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    void KeeperOfSecrets::onStartCombat(PlayerId player) {
        SlaaneshBase::onStartCombat(player);

        // Dark Temptations
        auto hero = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
        if (hero != nullptr) {
            // Heros _always_ refuse right now and take D3
            // TODO: give hero the choice
            hero->applyDamage({0, Dice::RollD3()}, this);
        }

        // Living Whip
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0);
        for (auto unit : units) {
            if (unit->hasKeyword(MONSTER)) {
                // TODO: pick a single melee weapon
                unit->buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Combat, m_battleRound, player});
            }
        }
    }

    std::string KeeperOfSecrets::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Ritual_Knife) return "Ritual Knife";
            else if (parameter.intValue == Sinistrous_Hand) return "Sinistrous Hand";
            else if (parameter.intValue == Living_Whip) return "LivingWhip";
            else if (parameter.intValue == Shining_Aegis) return "Shining Aegis";
        }
        return SlaaneshBase::ValueToString(parameter);
    }

    Wounds KeeperOfSecrets::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Delicate Precision
        if (!weapon->isMissile()) {
            if (woundRoll == 6) {
                return {0, weapon->damage()};
            }
        }
        return SlaaneshBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int KeeperOfSecrets::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ritual Knife") return Ritual_Knife;
        else if (enumString == "Sinistrous Hand") return Sinistrous_Hand;
        else if (enumString == "Living Whip") return Living_Whip;
        else if (enumString == "Shining Aegis") return Shining_Aegis;
        return SlaaneshBase::EnumStringToInt(enumString);
    }

    int KeeperOfSecrets::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
