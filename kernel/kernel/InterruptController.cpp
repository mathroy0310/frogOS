/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   InterruptController.cpp                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 17:48:10 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 17:48:16 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Errors.h>
#include <kernel/APIC.h>
#include <kernel/InterruptController.h>
#include <kernel/PIC.h>

static InterruptController *s_instance = nullptr;

InterruptController &InterruptController::Get() {
	ASSERT(s_instance);
	return *s_instance;
}

void InterruptController::Initialize(bool force_pic) {
	ASSERT(s_instance == nullptr);

	PIC::MaskAll();
	PIC::Remap();

	if (!force_pic) s_instance = APIC::Create();
	if (s_instance) return;
	dprintln("Using PIC instead of APIC");
	s_instance = PIC::Create();
}