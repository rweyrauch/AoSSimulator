/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/ShalaxiHelbane.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <slaanesh/Lore.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {

    class RefineSenses : public Spell {
    public:
        explicit RefineSenses(Unit* caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override;
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    RefineSenses::RefineSenses(Unit *caster) :
            Spell(caster, "Refine Senses", 4, 0) {
        m_allowedTargets = Abilities::Target::Self;
        m_effect = Abilities::EffectType::Buff;
    }

    Spell::Result RefineSenses::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {

        auto shalaxi = dynamic_cast<ShalaxiHelbane*>(m_caster);
        if (shalaxi) {
            shalaxi->enableRefineSenses();
        }
        return Spell::Result::Success;
    }

    static const int g_basesize = 100;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 310;

    struct TableEntry {
        int m_move;
        int m_soulpiercerWound;
        int m_clawDamage;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 2, 5},
                    {12, 2, 4},
                    {10, 3, 3},
                    {8,  3, 3},
                    {6,  4, 2}
            };

    bool ShalaxiHelbane::s_registered = false;

    ShalaxiHelbane::ShalaxiHelbane() :
            SlaaneshBase("Shalaxi Helbane", 14, g_wounds, 10, 4, false),
            m_livingWhip(Weapon::Type::Missile, "Living Whip", 6, 1, 3, 3, -1, 1),
            m_soulpiercer(Weapon::Type::Melee, "Soulpiercer", 3, 1, 2, 2, -3, RAND_D6),
            m_impalingClaws(Weapon::Type::Melee, "Impaling Claws", 3, 2, 3, 3, -2, 5) {
        m_keywords = {CHAOS, DAEMON, GREATER_DAEMON, SLAANESH, HEDONITE, MONSTER, HERO, WIZARD, KEEPER_OF_SECRETS,
                      SHALAXI_HELBANE};
        m_weapons = {&m_livingWhip, &m_soulpiercer, &m_impalingClaws};
        m_battleFieldRole = Leader_Behemoth;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool ShalaxiHelbane::configure(WeaponOption weapon, Lore lore) {
        auto model = new Model(g_basesize, wounds());

        m_weapon = weapon;

        if (m_weapon == Living_Whip) {
            model->addMissileWeapon(&m_livingWhip);
        }
        model->addMeleeWeapon(&m_soulpiercer);
        model->addMeleeWeapon(&m_impalingClaws);

        addModel(model);

        m_knownSpells.push_back(std::make_unique<RefineSenses>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *ShalaxiHelbane::Create(const ParameterList &parameters) {
        auto unit = new ShalaxiHelbane();
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Living_Whip);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greaterDaemonLore[0]);

        bool ok = unit->configure(weapon, lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ShalaxiHelbane::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Living_Whip, Shining_Aegis};
            static FactoryMethod factoryMethod = {
                    ShalaxiHelbane::Create,
                    ShalaxiHelbane::ValueToString,
                    ShalaxiHelbane::EnumStringToInt,
                    ShalaxiHelbane::ComputePoints,
                    {
                            EnumParameter("Weapon", Living_Whip, weapons),
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Lore", g_greaterDaemonLore[0], g_greaterDaemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Shalaxi Helbane", factoryMethod);
        }
    }

    void ShalaxiHelbane::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_soulpiercer.setToWound(g_damageTable[damageIndex].m_soulpiercerWound);
        m_impalingClaws.setDamage(g_damageTable[damageIndex].m_clawDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        Unit::onWounded();
    }

    int ShalaxiHelbane::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void ShalaxiHelbane::onRestore() {
        SlaaneshBase::onRestore();

        // Reset table-drive attributes
        onWounded();
    }

    Wounds ShalaxiHelbane::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        if (m_weapon == Shining_Aegis) {
            // Shining Aegis
            return ignoreWounds(wounds, 6);
        }
        return Unit::applyWoundSave(wounds, attackingUnit);
    }

    Wounds ShalaxiHelbane::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if (!weapon->isMissile()) {
            int damage = weapon->damage();
            // The Killing Stroke
            if ((target == m_meleeTarget) && (weapon->name() == m_soulpiercer.name())) {
                damage = 6;
            }

            // Delicate Precision
            if (woundRoll == 6) {
                return {0, damage};
            } else {
                return {damage, 0};
            }
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    std::string ShalaxiHelbane::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Living_Whip) return "LivingWhip";
            else if (parameter.intValue == Shining_Aegis) return "Shining Aegis";
        }
        return SlaaneshBase::ValueToString(parameter);
    }

    int ShalaxiHelbane::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Living Whip") return Living_Whip;
        else if (enumString == "Shining Aegis") return Shining_Aegis;
        return SlaaneshBase::EnumStringToInt(enumString);
    }

    int ShalaxiHelbane::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    Rerolls ShalaxiHelbane::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_refinedSensesActive && target->hasKeyword(HERO)) {
            return Reroll_Failed;
        }
        return SlaaneshBase::toHitRerolls(weapon, target);
    }

    Rerolls ShalaxiHelbane::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        if (m_refinedSensesActive && attacker->hasKeyword(HERO)) {
            return Reroll_Failed;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int ShalaxiHelbane::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        auto modifier = Unit::toSaveModifier(weapon, attacker);

        // Cloak of Constriction
        if (!weapon->isMissile() && attacker->hasKeyword(HERO)) {
            modifier++;
        }

        return modifier;
    }

    void ShalaxiHelbane::onStartCombat(PlayerId player) {
        EventInterface::onStartCombat(player);

        // Living Whip
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0);
        for (auto unit : units) {
            if (unit->hasKeyword(MONSTER)) {
                // TODO: pick a single melee weapon
                unit->buffModifier(To_Hit_Melee, -1, {Phase::Combat, m_battleRound, player});
            }
        }

        // The Killing Stroke
        if (meleeTarget() && (distanceTo(meleeTarget()) <= 3.0) && meleeTarget()->hasKeyword(HERO)) {
            m_soulpiercer.setDamage(6);
        }
        else {
            m_soulpiercer.setDamage(RAND_D6);
        }
    }

} // Slannesh
