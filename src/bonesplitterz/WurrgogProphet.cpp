/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Roster.h>
#include <spells/MysticShield.h>
#include "bonesplitterz/WurrgogProphet.h"
#include "BonesplitterzPrivate.h"

namespace Bonesplitterz {

    class FistsOfGork : public Spell {
    public:
        explicit FistsOfGork(Unit* caster);

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override;
        Result apply(int castingRoll, int unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    FistsOfGork::FistsOfGork(Unit *caster) :
            Spell(caster, "Fists of Gork", 5, 24) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result FistsOfGork::apply(int castingRoll, int unmodifiedCastingRoll, Unit *target) {
        if (target == nullptr) return Spell::Result::Failed;

        Dice::RollResult rolls;
        Dice::RollD6(target->remainingModels(), rolls);
        auto threshold = 6;
        if (castingRoll >= 10) {
            threshold = 4;
        }
        target->applyDamage({0, rolls.rollsGE(threshold), Wounds::Source::Spell}, m_caster);

        return Spell::Result::Success;
    }

    static const int g_basesize = 32;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 120;

    bool WurrgogProphet::s_registered = false;

    Unit *WurrgogProphet::Create(const ParameterList &parameters) {
        auto unit = new WurrgogProphet();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_prophetBossCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_wizardArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void WurrgogProphet::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_prophetBossCommandTraits[0], g_prophetBossCommandTraits),
                            EnumParameter("Artefact", g_wizardArtefacts[0], g_wizardArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };
            s_registered = UnitFactory::Register("Wurrgog Prophet", factoryMethod);
        }
    }

    WurrgogProphet::WurrgogProphet() :
            Bonesplitterz("Wurrgog Prophet", 5, g_wounds, 8, 5, false),
            m_staffAndShiv(Weapon::Type::Melee, "Wurrgog Staff and Bone Shiv", 1, 4, 4, 3, 0, RAND_D3),
            m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 1, RAND_D3, 4, 3, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, WIZARD, WURRGOG_PROPHET};
        m_weapons = {&m_staffAndShiv, &m_fangedMaw};
        m_battleFieldRole = Leader;
        m_totalUnbinds = 2;
        m_totalSpells = 2;
    }

    bool WurrgogProphet::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staffAndShiv);
        model->addMeleeWeapon(&m_fangedMaw);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<FistsOfGork>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int WurrgogProphet::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);
        // Beast Mask
        if (!weapon->isMissile()) {
            mod -= 1;
        }
        return mod;
    }

    void WurrgogProphet::onStartHero(PlayerId playerId) {
        Unit::onStartHero(playerId);

        if (owningPlayer() == playerId) {
            // Prophet of Da Waaagh!
            if (Dice::RollD6() >= 4) {
                m_roster->addCommandPoints(1);
            }
        }
    }

    int WurrgogProphet::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Bonesplitterz
