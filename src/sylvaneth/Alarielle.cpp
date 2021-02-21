/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Alarielle.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <sylvaneth/SylvanethSpells.h>
#include <Board.h>
#include <Roster.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {

    class GhyransWrath : public CommandAbility {
    public:
        explicit GhyransWrath(Unit *source);

    protected:
        bool apply(Unit* target) override;
        bool apply(double x, double y) override { return false; }
    };

    GhyransWrath::GhyransWrath(Unit *source) :
            CommandAbility(source, "Ghyran's Wrath", 14, 14, Phase::Combat) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords = {SYLVANETH};
    }

    bool GhyransWrath::apply(Unit* target) {

        auto units = Board::Instance()->getUnitsWithin(m_source->position(), m_source->owningPlayer(), m_rangeGeneral);
        for (auto unit : units) {
            if (unit->hasKeyword(SYLVANETH)) {
                unit->buffReroll(To_Wound_Melee, Reroll_Ones, defaultDuration());
            }
        }
        return true;
    }

    static const int g_basesize = 160;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 600;

    bool Alarielle::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_spearKurnothRange;
        int m_greatAntlerDamage;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {4, 7, 10, 13, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {16, 30, 5},
                    {14, 25, 4},
                    {10, 20, 3},
                    {8,  15, 2},
                    {6,  10, 1}
            };


    Alarielle::Alarielle() :
            SylvanethBase("Alarielle", 16, g_wounds, 10, 3, true),
            m_spearOfKurnoth(Weapon::Type::Missile, "Spear of Kurnoth", 30, 1, 3, 2, -2, RAND_D6),
            m_talonOfDwindling(Weapon::Type::Melee, "Talon of Dwindling", 1, 4, 3, 4, 0, 1),
            m_beetleGreatAntlers(Weapon::Type::Melee, "Great Antlers", 2, 5, 4, 3, -2, 5) {
        m_keywords = {ORDER, SYLVANETH, MONSTER, HERO, WIZARD, ALARIELLE_THE_EVERQUEEN};
        m_weapons = {&m_spearOfKurnoth, &m_talonOfDwindling, &m_beetleGreatAntlers};
        m_battleFieldRole = Leader_Behemoth;

        m_totalUnbinds = 3;
        m_totalSpells = 3;
    }

    Alarielle::~Alarielle() {
    }

    bool Alarielle::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_spearOfKurnoth);
        model->addMeleeWeapon(&m_talonOfDwindling);
        model->addMeleeWeapon(&m_beetleGreatAntlers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateMetamorphosis(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::make_unique<GhyransWrath>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int Alarielle::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        // Sweeping Blows
        if (weapon->name() == m_beetleGreatAntlers.name() && unit->remainingModels() >= 5) {
            return 1;
        }
        return Unit::toHitModifier(weapon, unit);
    }

    void Alarielle::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                // Lifebloom - heal herself D3
                int woundsHealed = Dice::RollD3();
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }

            // Lifebloom
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 30.0);
            for (auto ip : units) {
                if (ip->hasKeyword(SYLVANETH))
                    ip->heal(Dice::RollD3());
            }
        }
    }

    void Alarielle::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_spearOfKurnoth.setRange(g_damageTable[damageIndex].m_spearKurnothRange);
        m_beetleGreatAntlers.setDamage(g_damageTable[damageIndex].m_greatAntlerDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int Alarielle::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *Alarielle::Create(const ParameterList &parameters) {
        auto unit = new Alarielle();

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfTheDeepwood[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Alarielle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                            EnumParameter("Lore", g_loreOfTheDeepwood[0], g_loreOfTheDeepwood),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Alarielle", factoryMethod);
        }
    }

    void Alarielle::onCharged() {
        // Living Battering Ram
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            if (Dice::RollD6() >= 4) {
                Wounds wounds = {0, 0};
                wounds.mortal = Dice::RollD3();
                ip->applyDamage(wounds, this);
            }
        }
        Unit::onCharged();
    }

    Wounds Alarielle::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Talon of the Dwindling
        if (weapon->name() == m_talonOfDwindling.name() && hitRoll == 6) {
            return {weapon->damage(), Dice::RollD3()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Alarielle::onEndMovement(PlayerId player) {
        Unit::onEndMovement(player);

        if (owningPlayer() == player && !m_usedSoulAmphorae) {
            // Summon a unit and add to roster.
            auto factory = UnitFactory::LookupUnit("Kurnoth Hunters");
            if (factory) {
                if (m_roster) {
                    auto unit = UnitFactory::Create("Kurnoth Hunters", factory->m_parameters);
                    unit->deploy(position(), orientation());
                    m_roster->addUnit(unit);
                }
                m_usedSoulAmphorae = true;
            }
        }
    }

    void Alarielle::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int Alarielle::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Sylvaneth
