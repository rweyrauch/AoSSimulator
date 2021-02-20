/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "lumineth/Teclis.h"
#include "LuminethPrivate.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>

namespace LuminethRealmLords {

    class ProtectionOfTeclis : public Spell {
    public:
        explicit ProtectionOfTeclis(Unit* caster) :
            Spell(caster, "Protection of Teclis", 10, 18) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }
    protected:
        Result apply(int castingRoll, int unmodifiedCastingValue, double x, double y) override {
            auto teclis = dynamic_cast<ArchmageTeclis*>(m_caster);
            if (teclis) {
                teclis->enableProtectionOfTeclis();
            }
            return Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingValue, Unit* target) override {
            return apply(castingRoll, unmodifiedCastingValue, 0, 0);
        }
    };

    class StormOfSearingWhiteLight : public Spell {
    public:
        explicit StormOfSearingWhiteLight(Unit* caster) :
                Spell(caster, "StormOfSearingWhiteLight", 10, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::AreaOfEffectDamage;
        }

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), m_range);
            for (auto unit : units) {
                auto roll = Dice::RollD6();
                if (roll >= 5) {
                    unit->applyDamage({0, Dice::RollD6(), Wounds::Source::Spell}, m_caster);
                }
                else if (roll >= 2) {
                    unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
                }
            }
            return Spell::Result::Success;
        }
        Result apply(int castingRoll, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    static const int g_basesize = 100;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 660;

    struct TableEntry {
        int m_move;
        int m_talonAttacks;
        int m_auraRange;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {4, 7, 10, 13, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 6, 16},
                    {10,  5,  12},
                    {8,  4,  8},
                    {6,  3,  6},
                    {4,  2,  4}
            };

    bool ArchmageTeclis::s_registered = false;

    Unit *ArchmageTeclis::Create(const ParameterList &parameters) {
        auto unit = new ArchmageTeclis();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto nation = (GreatNation)GetEnumParam("Nation", parameters, (int)GreatNation::None);
        unit->setNation(nation);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreTeclis[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int ArchmageTeclis::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void ArchmageTeclis::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Lore", g_loreTeclis[0], g_loreTeclis),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Archmage Teclis", factoryMethod);
        }
    }

    ArchmageTeclis::ArchmageTeclis() :
            LuminethBase("Archmage Teclis", 12, g_wounds, 10, 4, true),
            m_staff(Weapon::Type::Missile, "Lunar Staff", 18, 1, 2, 2, -3, RAND_D3),
            m_sword(Weapon::Type::Melee, "Sword of Teclis", 1, 2, 4, 2, -3, RAND_D3),
            m_talons(Weapon::Type::Melee, "Moonbright Talons", 1, 6, 3, 3, -2, 2) {
        m_weapons = {&m_staff, &m_sword, &m_talons};
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, MONSTER, HERO, WIZARD, TECLIS};
        m_totalUnbinds = INT32_MAX;
        m_totalSpells = 4;
        m_hasMount = true;
        m_battleFieldRole = Role::Leader;

        s_globalCastMod.connect(this, &ArchmageTeclis::auraOfCelennar, &m_auraConnection);
        s_globalWoundSave.connect(this, &ArchmageTeclis::protectionAuras, &m_protectionConnection);
    }

    ArchmageTeclis::~ArchmageTeclis() {
        m_auraConnection.disconnect();
        m_protectionConnection.disconnect();
    }

    bool ArchmageTeclis::configure(Lore lore) {

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_staff);
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<ProtectionOfTeclis>(this));
        m_knownSpells.push_back(std::make_unique<StormOfSearingWhiteLight>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    void ArchmageTeclis::onWounded() {
        Unit::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_move = g_damageTable[damageIndex].m_move;
        m_talons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);
    }

    void ArchmageTeclis::onRestore() {
        LuminethBase::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int ArchmageTeclis::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int ArchmageTeclis::auraOfCelennar(const Unit *caster) {
        // Aura of Celennar
        if (caster->hasKeyword(LUMINETH_REALM_LORDS) &&
            (caster->owningPlayer() == owningPlayer()) && (distanceTo(caster) <= g_damageTable[getDamageTableIndex()].m_auraRange)) {
            return 1;
        }
        return 0;
    }

    Wounds ArchmageTeclis::protectionAuras(const Wounds& wounds, const Unit* target, const Unit* attacker) {
        Wounds totalWounds = wounds;

        if (!isFriendly(attacker)) {
            // Seeing Stone of Celennar
            if ((wounds.source == Wounds::Source::Spell) && isFriendly(target) &&
                (distanceTo(target) <= g_damageTable[getDamageTableIndex()].m_auraRange)) {
                // Unit has a 4+ ignore wounds
                if (Dice::RollD6() >= 4) {
                    totalWounds.mortal = 0;
                    totalWounds.normal = 0;

                    auto unit = Board::Instance()->getNearestUnit(target, attacker->owningPlayer());
                    if (unit && distanceTo(unit) <= 18) {
                        // TODO: apply damage to the unit without locking up in the lsignal callback!
                        //unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, this);
                    }
                }
            }

            // Protection of Teclis
            if (isFriendly(target) && (distanceTo(target) <= 18.0)) {
                // Unit has a 5+ ignore wounds
                Dice::RollResult woundSaves, mortalSaves;
                Dice::RollD6(totalWounds.normal, woundSaves);
                Dice::RollD6(totalWounds.mortal, mortalSaves);

                totalWounds.normal -= woundSaves.rollsGE(5);
                totalWounds.mortal -= mortalSaves.rollsGE(5);
                totalWounds.clamp();
            }
        }

        return totalWounds;
    }

    void ArchmageTeclis::onStartHero(PlayerId player) {
        LuminethBase::onStartHero(player);

        if (player == owningPlayer()) {
            // TODO: Determine how many spells to cast this turn: 1, 2 or 4.
            m_totalSpells = 4;

            // Disable 'Protection of Teclis' at the start of hero phase.
            m_protectionOfTeclisEnabled = false;
        }
    }

    void ArchmageTeclis::enableProtectionOfTeclis() {
        m_protectionOfTeclisEnabled = true;
    }

    int ArchmageTeclis::rollCasting(int &unmodifiedRoll) const {
        // Teclis ignores all casting modifiers.
        if (m_totalSpells >= 4) {
            unmodifiedRoll = 10;
        }
        else if (m_totalSpells >= 2) {
            unmodifiedRoll = 12;
        }
        else { // m_totalSpells == 1
            unmodifiedRoll = Automatically_Cast_No_Unbind;
        }
        return unmodifiedRoll;
    }

} // namespace LuminethRealmLords