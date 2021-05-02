/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/OgroidThaumaturge.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {

    class ChokingTendrils : public Spell {
    public:
        explicit ChokingTendrils(Unit *caster);

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override;

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    ChokingTendrils::ChokingTendrils(Unit *caster) :
            Spell(caster, "Choking Tendrils", 7, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result
    ChokingTendrils::apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        auto numSlain = target->applyDamage({0, Dice::RollSpecial(RAND_D6), Wounds::Source::Spell}, m_caster);
        if (numSlain > 0) {
            m_caster->heal(numSlain);
        }

        return Spell::Result::Success;
    }

    static const int g_basesize = 50;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 160;

    bool OgroidThaumaturge::s_registered = false;

    Unit *OgroidThaumaturge::Create(const ParameterList &parameters) {
        auto unit = new OgroidThaumaturge();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);
        unit->setChangeCoven(coven);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_arcaniteCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_arcaniteArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfFate[0]);

        unit->configure(lore);
        return unit;
    }

    void OgroidThaumaturge::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OgroidThaumaturge::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    OgroidThaumaturge::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_arcaniteCommandTraits[0], g_arcaniteCommandTraits),
                            EnumParameter("Artefact", g_arcaniteArtefacts[0], g_arcaniteArtefacts),
                            EnumParameter("Lore", g_loreOfFate[0], g_loreOfFate),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Ogroid Thaumaturge", factoryMethod);
        }
    }

    OgroidThaumaturge::OgroidThaumaturge() :
            TzeentchBase("OgroidT haumaturge", 6, g_wounds, 8, 5, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Thaumaturge Staff", 2, 3, 3, 3, -1, RAND_D3),
            m_horns(Weapon::Type::Melee, "Great Horns", 1, 2, 3, 3, -2, 3),
            m_hooves(Weapon::Type::Melee, "Cloven Hooves", 1, 4, 4, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, OGROID_THAUMATURGE};
        m_weapons = {&m_staff, &m_horns, &m_hooves};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    void OgroidThaumaturge::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<ChokingTendrils>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int OgroidThaumaturge::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void OgroidThaumaturge::onCharged() {
        // Mighty Rampage
        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            int roll = Dice::RollD6();
            if (roll >= 2) {
                Wounds wounds = {0, Dice::RollD3()};
                m_meleeTarget->applyDamage(wounds, this);
            }
        }
        TzeentchBase::onCharged();
    }

    Rerolls OgroidThaumaturge::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Brutal Rage
        if (!m_currentRecord.m_woundsTaken.zero() && weapon->isMelee()) {
            return Rerolls::Failed;
        }
        return TzeentchBase::toWoundRerolls(weapon, target);
    }

    Rerolls OgroidThaumaturge::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Brutal Rage
        if (!m_currentRecord.m_woundsTaken.zero() && weapon->isMelee()) {
            return Rerolls::Failed;
        }
        return TzeentchBase::toHitRerolls(weapon, target);
    }

} // Tzeentch