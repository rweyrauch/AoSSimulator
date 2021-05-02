/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "idonethdeepkin/IsharannTidecaster.h"
#include "IdonethDeepkinPrivate.h"
#include "IdonethLore.h"

namespace IdonethDeepkin {

    class Riptide : public Spell {
    public:
        explicit Riptide(Unit *caster) :
                Spell(caster, "Riptide", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            target->buffModifier(Attribute::To_Hit_Missile, -1, defaultDuration());
            target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());

            auto tc = dynamic_cast<IsharannTidecaster *>(m_caster);
            if (tc != nullptr) {
                tc->setRiptideTarget(target);
            }

            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool IsharannTidecaster::s_registered = false;

    Unit *IsharannTidecaster::Create(const ParameterList &parameters) {
        auto unit = new IsharannTidecaster();

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_arcaneArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        unit->configure(lore);
        return unit;
    }

    void IsharannTidecaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_arcaneArtefacts[0], g_arcaneArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Isharann Tidecaster", factoryMethod);
        }
    }

    IsharannTidecaster::IsharannTidecaster() :
            IdonethDeepkinBase("Isharann Tidecaster", 6, g_wounds, 7, 6, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Pelagic Staff", 1, 2, 3, 3, 0, RAND_D3) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, ISHARANN, HERO, WIZARD, TIDECASTER};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    void IsharannTidecaster::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);

        addModel(model);

        m_knownSpells.push_back(std::make_unique<Riptide>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    int IsharannTidecaster::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void IsharannTidecaster::onStartHero(PlayerId player) {
        EventInterface::onStartHero(player);

        if (owningPlayer() == player) {
            if (m_riptideTarget != nullptr) {
                m_riptideTarget->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, this);
                m_riptideTarget = nullptr;
            }
        }
    }

} //IdonethDeepkin