/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "nighthaunt/SpiritTorment.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool SpiritTorment::s_registered = false;

    Unit *SpiritTorment::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new SpiritTorment(trait, artefact, general);
    }

    void SpiritTorment::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpiritTorment::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    SpiritTorment::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Spirit Torment", factoryMethod);
        }
    }

    SpiritTorment::SpiritTorment(CommandTrait trait, Artefact artefact, bool isGeneral) :
            Nighthaunt("Spirit Torment", 6, g_wounds, 10, 4, true) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, SPIRIT_TORMENT};
        m_weapons = {&m_chains};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        s_globalToHitReroll.connect(this, &SpiritTorment::nagashsBidding, &m_nagashsBiddingSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_chains);
        addModel(model);

        m_points = ComputePoints(1);
    }

    SpiritTorment::~SpiritTorment() {
        m_nagashsBiddingSlot.disconnect();
    }

    int SpiritTorment::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    Rerolls SpiritTorment::nagashsBidding(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (isFriendly(attacker) && attacker->hasKeyword(NIGHTHAUNT) && (distanceTo(attacker) < 12.0)) {
            return Rerolls::Ones;
        }
        return Rerolls::None;
    }

} // namespace Nighthaunt
