/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LUMINETHREALMLORDS_H
#define LUMINETHREALMLORDS_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace LuminethRealmLords {

    enum class GreatNation : int {
        None,

        Syar,
        Iliatha,
        Zaitrec,
        Ymetrica,
    };

    enum class CommandTrait : int {
        None,
        Lords_of_Brilliance,
        Lords_of_Stone,

        // Scinari
        Spellmaster,
        Loremaster,
        Warmaster,

        // Alarith
        Majestic,
        Enduring,
        //Loremaster,

        // Nation specific
        Almighty_Blow, // Ymetrica
        Goading_Arrogance, // Syar
        Fast_Learner, // Zaitrec
    };

    enum class Artefact : int {
        None,

        // Scinari
        Phoenix_Stone,
        Silver_Wand,
        Blade_of_Leaping_Gold,

        // Alarith
        Heartstone_Amulet,
        Ebony_Stone,
        Magmic_Hammer,

        // Nation specific
        Mountains_Gift, // Ymetrica
        The_Perfect_Blade, // Syar
        Simulacra_Amulet, // Iliatha
        Gift_of_Celennar, // Zaitrec
    };

    enum class Lore : int {
        None,

        // Lore of Hysh
        Speed_of_Hysh,
        Solar_Flare,
        Lambent_Light,
        Etheral_Blessing,
        Total_Eclipse,
        Protection_of_Hysh,

        // Lore of the Mountains
        Unyielding_Calm,
        Crippling_Vertigo,
        Voice_of_the_Mountains,
        Living_Fissue,
        Entomb,
        Assault_of_Stone,

        // Nation specific
        Overwhelming_Heat, // Zaitrec
    };

    class LuminethBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        LuminethBase();

        ~LuminethBase() override;

        void setNation(GreatNation nation);

        void setCommandTrait(CommandTrait trait);
        void setArtefact(Artefact artefact);
    protected:

        LuminethBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int braveryModifier() const override;
        int woundModifier() const override;

        int majestic(const Unit *unit);

    protected:

        GreatNation m_nation = GreatNation::None;

        CommandTrait m_commandTrait = CommandTrait::None;
        Artefact m_artefact = Artefact::None;

        int m_aetherQuartzReserve = 1;

        lsignal::slot m_majesticConnection;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Aetherquartz Reserve             TODO
//   Heightened Reflexes            TODO
//   Heighteden Senses              TODO
//   Magical Boost                  TODO
//   Magical Insight                TODO
// Absorb Despair                   TODO
// Lightning Reactions              TODO
// Shining Company                  TODO
// Enduring as Rock                 TODO
// Tectonic Force                   TODO
// Mountain Realm                   TODO
// Gleaming Brightness              TODO
// Soul-bond                        TODO
// Lambent Mystics                  TODO
// Unity of Purpose                 TODO
//
    void Init();

} // namespace LuminethRealmLords

#endif // LUMINETHREALMLORDS_H
