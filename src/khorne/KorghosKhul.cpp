/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/KorghosKhul.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {

    class LordOfTheGoretide : public CommandAbility {
    public:
        explicit LordOfTheGoretide(Unit *source);

    protected:
        bool apply(Unit *target) override;

        bool apply(double x, double y) override { return false; }
    };

    LordOfTheGoretide::LordOfTheGoretide(Unit *source) :
            CommandAbility(source, "Lord of the Goretide", 16, 16, Phase::Charge) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords = {GORETIDE};
    }

    bool LordOfTheGoretide::apply(Unit *target) {

        auto units = Board::Instance()->getUnitsWithin(m_source->position(), m_source->owningPlayer(), m_rangeGeneral);
        for (auto unit : units) {
            if (unit->hasKeyword(GORETIDE)) {
                unit->buffReroll(Attribute::To_Hit_Melee, Rerolls::Ones,
                                 {Phase::Combat, m_round, m_source->owningPlayer()});
            }
        }
        return true;
    }

    static const int g_basesize = 60;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 180;

    bool KorghosKhul::s_registered = false;

    KorghosKhul::KorghosKhul(bool isGeneral) :
            KhorneBase("Korghos Khul", 5, g_wounds, 9, 3, false),
            m_axeOfKhorne(Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 4, 3, 4, -1, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, GORETIDE, HERO, MIGHTY_LORD_OF_KHORNE, KORGHOS_KHUL};
        m_weapons = {&m_axeOfKhorne, &m_clawsAndFangs};
        m_battleFieldRole = Role::Leader;

        // Collar of Khorne
        m_totalUnbinds = 1;

        // Aqshy's Bane
        m_pileInMove = 8;

        setSlaughterHost(SlaughterHost::Goretide);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_axeOfKhorne);
        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<LordOfTheGoretide>(this));

        m_points = g_pointsPerUnit;
    }

    Unit *KorghosKhul::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);

        return new KorghosKhul(general);
    }

    void KorghosKhul::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KorghosKhul::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    KorghosKhul::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Korghos Khul", factoryMethod);
        }
    }

    Rerolls KorghosKhul::toHitRerolls(const Weapon * /*weapon*/, const Unit * /*target*/) const {
        // Favoured of Khorne
        return Rerolls::Failed;
    }

    int KorghosKhul::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void KorghosKhul::onEndCombat(PlayerId player) {

        KhorneBase::onEndCombat(player);
    }

    void KorghosKhul::onEnemyModelWoundedWithWeapon(Unit *enemyUnit, const Weapon *weapon,
                                                    const Wounds &weaponDamage) {
        KhorneBase::onEnemyModelWoundedWithWeapon(enemyUnit, weapon, weaponDamage);

        if (weapon->name() == m_axeOfKhorne.name()) {
            // Reality-splitting Axe
            if (Dice::RollD6() >= 5) {
                PLOG_INFO << name() << " slays wounded model from enemy unit " << enemyUnit->name() << " with " <<
                          weapon->name() << " Reality-splitting Axe ability.";
                enemyUnit->slay(1);
            }
        }
    }


} // namespace Khorne