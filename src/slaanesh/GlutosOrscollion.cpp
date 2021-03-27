/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "slaanesh/GlutosOrscollion.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <slaanesh/Lore.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {

    class CripplingFamishment : public Spell {
    public:
        explicit CripplingFamishment(Unit *caster) :
                Spell(caster, "Crippling Famishment", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            target->buffMovement(MovementRule::Halve_Movement, true, defaultDuration());
            target->buffMovement(MovementRule::Halve_Run_Roll, true, defaultDuration());
            target->buffMovement(MovementRule::Halve_Charge_Roll, true, defaultDuration());
            return Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    static const int g_basesize = 100;
    static const int g_wounds = 18;
    static const int g_pointsPerUnit = 400;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 4;
    static int g_woundThresholds[g_numTableEntries] = {4, 8, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 6},
                    {6, 5},
                    {5, 4},
                    {4, 3},
            };

    bool GlutosOrscollion::s_registered = false;

    GlutosOrscollion::GlutosOrscollion() :
            SlaaneshBase("Glutos Orscollion", 14, g_wounds, 10, 4, false),
            m_greatblade(Weapon::Type::Melee, "Wailing Greatblade", 1, 3, 3, 3, -2, 2),
            m_scourge(Weapon::Type::Melee, "Flaying Scourge", 1, 2, 3, 4, 0, 1),
            m_dagger(Weapon::Type::Melee, "Sacrificial Dagger", 1, 1, 4, 3, 0, 1),
            m_claws(Weapon::Type::Melee, "Crushing Claws", 1, 6, 3, 3, -1, 3) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HERO, WIZARD, GLUTOS_ORSCOLLION};
        m_weapons = {&m_greatblade, &m_scourge, &m_dagger, &m_claws};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_claws.setMount(true);

        m_totalSpells = 2;
        m_totalUnbinds = 2;

        s_globalToHitMod.connect(this, &GlutosOrscollion::fogOfTemptation, &m_fogConnection);
        s_globalBraveryMod.connect(this, &GlutosOrscollion::aperitif, &m_aperitifConnection);
    }

    GlutosOrscollion::~GlutosOrscollion() {
        m_fogConnection.disconnect();
        m_aperitifConnection.disconnect();
    }

    void GlutosOrscollion::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());

        model->addMeleeWeapon(&m_greatblade);
        model->addMeleeWeapon(&m_scourge);
        model->addMeleeWeapon(&m_dagger);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<CripplingFamishment>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    Unit *GlutosOrscollion::Create(const ParameterList &parameters) {
        auto unit = new GlutosOrscollion();
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greaterDaemonLore[0]);

        unit->configure(lore);
        return unit;
    }

    void GlutosOrscollion::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GlutosOrscollion::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    GlutosOrscollion::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Lore", g_greaterDaemonLore[0], g_greaterDaemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Glutos Orscollion", factoryMethod);
        }
    }

    void GlutosOrscollion::onWounded() {
        const auto damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        if (woundsTaken() >= 13) {
            m_greatblade.activate(false);
        } else if (woundsTaken() >= 8) {
            m_scourge.activate(false);
        } else if (woundsTaken() >= 4) {
            m_dagger.activate(false);
        }
        SlaaneshBase::onWounded();
    }

    size_t GlutosOrscollion::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void GlutosOrscollion::onRestore() {
        SlaaneshBase::onRestore();

        // Reset table-drive attributes
        onWounded();

        m_runAndCharge = false;

        m_greatblade.activate(true);
        m_scourge.activate(true);
        m_dagger.activate(true);
    }

    int GlutosOrscollion::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int GlutosOrscollion::castingModifier() const {
        auto mod = SlaaneshBase::castingModifier();
        // The Leerstave of Loth'shar
        mod++;
        return mod;
    }

    int GlutosOrscollion::unbindingModifier() const {
        auto mod = SlaaneshBase::unbindingModifier();
        // The Leerstave of Loth'shar
        mod++;
        return mod;
    }

    Wounds GlutosOrscollion::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Painbringer Kyazu
        if ((hitRoll == 6) && (weapon->name() == m_greatblade.name())) {
            return {0, 2, Wounds::Source::Weapon_Melee};
        }
        return SlaaneshBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls GlutosOrscollion::chargeRerolls() const {
        // Lashmaster Vhyssk
        if (m_scourge.isActive()) {
            return Rerolls::Failed;
        }
        return SlaaneshBase::chargeRerolls();
    }

    void GlutosOrscollion::onStartHero(PlayerId player) {
        SlaaneshBase::onStartHero(player);

        m_protectedByDolece = false;
        // Priestess Dolece
        if (m_dagger.isActive()) {
            if (Dice::RollD6() >= 2) {
                m_protectedByDolece = true;
            }
        }
    }

    Wounds GlutosOrscollion::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = wounds;
        if (m_protectedByDolece) {
            totalWounds = ignoreWounds(totalWounds, 6);
        }
        return SlaaneshBase::applyWoundSave(totalWounds, attackingUnit);
    }

    int GlutosOrscollion::fogOfTemptation(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (!isFriendly(attacker) && distanceTo(attacker) < 12.0) {
            return -1;
        }
        return 0;
    }

    int GlutosOrscollion::aperitif(const Unit *unit) {
        if (isFriendly(unit) && unit->hasKeyword(MORTAL) && unit->hasKeyword(HEDONITE) && (distanceTo(unit) <= 6.0)) {
            return 1;
        }
        return 0;
    }

    void GlutosOrscollion::onBeginRound(int battleRound) {
        SlaaneshBase::onBeginRound(battleRound);

        // Starter
        m_runAndCharge = (battleRound >= 2);
    }

    void GlutosOrscollion::onStartBattleshock(PlayerId player) {
        SlaaneshBase::onStartBattleshock(player);

        // Main Course
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
        for (auto unit : units) {
            unit->buffAbility(Ability::Ignore_Battleshock, true, {Phase::Battleshock, m_battleRound, player});
        }
    }

    void GlutosOrscollion::onEndHero(PlayerId player) {
        SlaaneshBase::onEndHero(player);

        // Dessert
        if (owningPlayer() == player) {
            m_totalSpells = (m_battleRound >= 4) ? 2 : 1;
            // TODO: Can also select a new Lore
        }
    }

    Rerolls GlutosOrscollion::castingRerolls() const {
        // Digestif
        return (m_battleRound >= 5) ? Rerolls::Failed : Unit::castingRerolls();
    }

    Rerolls GlutosOrscollion::unbindingRerolls() const {
        // Digestif
        return (m_battleRound >= 5) ? Rerolls::Failed : Unit::unbindingRerolls();
    }

} // Slannesh
