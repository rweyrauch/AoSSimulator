/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <magic_enum.hpp>
#include <kharadron/KharadronBase.h>
#include <kharadron/AethericNavigator.h>
#include <kharadron/AetherKhemist.h>
#include <kharadron/ArkanautAdmiral.h>
#include <kharadron/ArkanautCompany.h>
#include <kharadron/ArkanautFrigate.h>
#include <kharadron/ArkanautIronclad.h>
#include <kharadron/BrokkGrungsson.h>
#include <kharadron/EndrinmasterWithHarness.h>
#include <kharadron/EndrinmasterWithSuit.h>
#include <kharadron/Endrinriggers.h>
#include <kharadron/GrundstokGunhauler.h>
#include <kharadron/GrundstokThunderers.h>
#include <kharadron/Skywardens.h>
#include "kharadron/BjorgenThundrik.h"
#include "kharadron/ThundriksProfiteers.h"

namespace KharadronOverlords {

    std::string KharadronBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Skyport") {
            auto portName = magic_enum::enum_name((Skyport)parameter.intValue);
            return std::string(portName);
        }
        if (std::string(parameter.name) == "Artycle") {
            auto artycleName = magic_enum::enum_name((Artycle)parameter.intValue);
            return std::string(artycleName);
        }
        if (std::string(parameter.name) == "Amendment") {
            auto amendmentName = magic_enum::enum_name((Amendment)parameter.intValue);
            return std::string(amendmentName);
        }
        if (std::string(parameter.name) == "Footnote") {
            auto noteName = magic_enum::enum_name((Footnote)parameter.intValue);
            return std::string(noteName);
        }
        if (std::string(parameter.name) == "Command Trait") {
            auto traitName = magic_enum::enum_name((CommandTrait)parameter.intValue);
            return std::string(traitName);
        }
        if (std::string(parameter.name) == "Artefact") {
            auto artefactName = magic_enum::enum_name((Artefact)parameter.intValue);
            return std::string(artefactName);
        }

        return ParameterValueToString(parameter);
    }

    int KharadronBase::EnumStringToInt(const std::string &enumString) {
        auto port = magic_enum::enum_cast<Skyport>(enumString);
        if (port.has_value()) return (int)port.value();

        auto artycle = magic_enum::enum_cast<Artycle>(enumString);
        if (artycle.has_value()) return (int)artycle.value();

        auto amendment = magic_enum::enum_cast<Amendment>(enumString);
        if (amendment.has_value()) return (int)amendment.value();

        auto note = magic_enum::enum_cast<Footnote>(enumString);
        if (note.has_value()) return (int)note.value();

        auto trait = magic_enum::enum_cast<CommandTrait>(enumString);
        if (trait.has_value()) return (int)trait.value();

        auto artefact = magic_enum::enum_cast<Artefact>(enumString);
        if (artefact.has_value()) return (int)artefact.value();

        return 0;
    }

    void KharadronBase::setSkyport(Skyport skyport) {
        removeKeyword(BARAK_NAR);
        removeKeyword(BARAK_ZON);
        removeKeyword(BARAK_ZILFIN);
        removeKeyword(BARAK_URBAZ);
        removeKeyword(BARAK_THRYNG);
        removeKeyword(BARAK_MHORNAR);

        m_skyport = skyport;
        switch (skyport) {
            case Skyport::Barak_Nar:
                addKeyword(BARAK_NAR);
                setCode(Artycle::Respect_Your_Commanders, Amendment::Trust_Aethermatics_Not_Superstition, Footnote::Through_Knowledge_Power);
                break;
            case Skyport::Barak_Zon:
                addKeyword(BARAK_ZON);
                setCode(Artycle::Honour_Is_Everything, Amendment::Leave_No_Duardin_Behind, Footnote::Show_Them_Your_Steel);
                break;
            case Skyport::Barak_Zilfin:
                addKeyword(BARAK_ZILFIN);
                setCode(Artycle::Master_the_Skies, Amendment::Dont_Argue_With_the_Wind, Footnote::Theres_Always_a_Breeze);
                break;
            case Skyport::Barak_Urbaz:
                addKeyword(BARAK_URBAZ);
                setCode(Artycle::Seek_New_Prospects, Amendment::Always_Take_What_You_Are_Owed, Footnote::Where_Theres_War_Theres_Gold);
                break;
            case Skyport::Barak_Thryng:
                addKeyword(BARAK_THRYNG);
                setCode(Artycle::Chronicle_of_Grudges, Amendment::Take_Help_Where_You_Can_Get_It, Footnote::Honour_the_Gods_Just_in_Case);
                break;
            case Skyport::Barak_Mhornar:
                addKeyword(BARAK_MHORNAR);
                setCode(Artycle::Seek_New_Prospects, Amendment::Prosecute_Wars_With_All_Haste, Footnote::Who_Strikes_First_Strikes_Hardest);
                break;
            default:
                break;
        }
    }

    void KharadronBase::setCode(Artycle artycle, Amendment amendment,
                                Footnote footnote) {
        if (m_skyport != Skyport::None) {
            // Error.
        }
        m_artycle = artycle;
        m_amendment = amendment;
        m_footnote = footnote;
    }

    void KharadronBase::setCommandTrait(CommandTrait trait) {
        m_commandTrait = trait;
    }

    void KharadronBase::setArtefact(Artefact artefact) {
        m_artefact = artefact;
    }

    void Init() {
        AethericNavigator::Init();
        AetherKhemist::Init();
        ArkanautAdmiral::Init();
        ArkanautCompany::Init();
        ArkanautFrigate::Init();
        ArkanautIronclad::Init();
        BrokkGrungsson::Init();
        EndrinmasterWithDirigibleSuit::Init();
        EndrinmasterWithEndrinharness::Init();
        Endrinriggers::Init();
        GrundstokGunhauler::Init();
        GrundstokThunderers::Init();
        Skywardens::Init();
        BjorgenThundrik::Init();
        ThundriksProfiteers::Init();
    }

} //KharadronOverlords
