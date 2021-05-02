/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "lumineth/Stonemage.h"
#include "LuminethPrivate.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>

namespace LuminethRealmLords {

    class GraviticRedirection : public Spell {
    public:
        explicit GraviticRedirection(Unit *caster) :
                Spell(caster, "Gravitic Redirection", 5, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr) return Result::Failed;

            target->applyDamage({0, 1, Wounds::Source::Spell}, m_caster);
            target->buffMovement(MovementRule::Halve_Movement, true, defaultDuration());
            target->buffMovement(MovementRule::Can_Fly, false, defaultDuration());

            return Result::Success;
        }
    };

    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 130;

    bool AlarithStonemage::s_registered = false;

    Unit *AlarithStonemage::Create(const ParameterList &parameters) {
        auto unit = new AlarithStonemage();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraitsAlarith[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_commandTraitsAlarith[0]);
        unit->setArtefact(artefact);

        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        unit->setNation(nation);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfHighPeaks[0]);

        unit->configure(lore);
        return unit;
    }

    int AlarithStonemage::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void AlarithStonemage::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Lore", g_loreOfHighPeaks[0], g_loreOfHighPeaks),
                            EnumParameter("Command Trait", g_commandTraitsAlarith[0], g_commandTraitsAlarith),
                            EnumParameter("Artefact", g_artefactsAlarith[0], g_artefactsAlarith),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Alarith Stonemage", factoryMethod);
        }
    }

    AlarithStonemage::AlarithStonemage() :
            LuminethBase("Alarith Stonemage", 6, g_wounds, 8, 5, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Staff of the High Peaks", 3, RAND_D3, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, AELEMENTIRI, ALARITH, HERO, WIZARD, STONEMAGE};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    void AlarithStonemage::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<GraviticRedirection>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void AlarithStonemage::onStartCombat(PlayerId player) {
        LuminethBase::onStartCombat(player);
        if (player != owningPlayer()) return;

        // Stonemage Stance
        if (meleeTarget() && distanceTo(meleeTarget()) < 3.0) {
            buffMovement(MovementRule::Can_PileIn, false, {GamePhase::Combat, m_battleRound, player});
            buffModifier(Attribute::Weapon_Rend_Melee, -1, {GamePhase::Combat, m_battleRound, player});

            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
            for (auto unit : units) {
                if (unit->hasKeyword(STONEGUARD) && unit->hasKeyword(ALARITH)) {
                    if (unit->meleeTarget() && unit->distanceTo(unit->meleeTarget()) < 1.0) {
                        unit->buffMovement(MovementRule::Can_PileIn, false, {GamePhase::Combat, m_battleRound, player});
                        unit->buffModifier(Attribute::Weapon_Rend_Melee, -1, {GamePhase::Combat, m_battleRound, player});
                    }
                }
            }
        }
    }

} // namespace LuminethRealmLords