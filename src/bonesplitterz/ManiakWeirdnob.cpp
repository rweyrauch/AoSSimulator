/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "bonesplitterz/ManiakWeirdnob.h"
#include "BonesplitterzPrivate.h"

namespace Bonesplitterz {
    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 140;

    bool ManiakWeirdnob::s_registered = false;

    Unit *ManiakWeirdnob::Create(const ParameterList &parameters) {
        auto unit = new ManiakWeirdnob();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_wizardCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_wizardArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        unit->configure(lore);
        return unit;
    }

    void ManiakWeirdnob::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_wizardCommandTraits[0], g_wizardCommandTraits),
                            EnumParameter("Artefact", g_wizardArtefacts[0], g_wizardArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Maniak Weirdnob", factoryMethod);
        }
    }

    ManiakWeirdnob::ManiakWeirdnob() :
            Bonesplitterz("Maniak Weirdnob", 12, g_wounds, 7, 6, false),
            m_bonebeastStaff(Weapon::Type::Melee, "Bonebeast Staff", 1, 3, 4, 3, 0, RAND_D3),
            m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, WIZARD, MANIAK_WEIRDNOB};
        m_weapons = {&m_bonebeastStaff, &m_tusksAndHooves};
        m_hasMount = true;
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    void ManiakWeirdnob::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_bonebeastStaff);
        model->addMeleeWeapon(&m_tusksAndHooves);

        addModel(model);

        m_knownSpells.push_back(
                std::make_unique<BuffAbilitySpell>(this, "Bone Spirit", 7, 12, Ability::Extra_Hit_On_Value, 6,
                                                   Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    int ManiakWeirdnob::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int ManiakWeirdnob::toHitModifier(const Weapon *weapon, const Unit *target) const {
        // Tusker Charge
        auto mod = Bonesplitterz::toHitModifier(weapon, target);
        if (m_charged && weapon->name() == m_tusksAndHooves.name()) {
            mod++;
        }
        return mod;
    }

    int ManiakWeirdnob::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        // Tusker Charge
        auto mod = Bonesplitterz::toWoundModifier(weapon, target);
        if (m_charged && weapon->name() == m_tusksAndHooves.name()) {
            mod++;
        }
        return mod;
    }

    void ManiakWeirdnob::onBeginTurn(int battleRound, PlayerId player) {
        Bonesplitterz::onBeginTurn(battleRound, player);

        m_usedWeirdSquig = false;
    }

    Rerolls ManiakWeirdnob::castingRerolls() const {
        if (!m_usedWeirdSquig) {
            m_usedWeirdSquig = true;
            return Rerolls::Failed;
        }
        return Bonesplitterz::castingRerolls();
    }

    Rerolls ManiakWeirdnob::unbindingRerolls() const {
        if (!m_usedWeirdSquig) {
            m_usedWeirdSquig = true;
            return Rerolls::Failed;
        }
        return Bonesplitterz::unbindingRerolls();
    }

} // namespace Bonesplitterz
