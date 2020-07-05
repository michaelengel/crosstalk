//
// kernel.cpp
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014-2018  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "kernel.h"
#include <assert.h>
#include <circle/usb/usbkeyboard.h>
#include <circle/string.h>
#include <circle/util.h>
#include <smalltalk.h>

#define PARTITION       "emmc1-1"

static const char ClearScreen[] = "\x1b[H\x1b[J\x1b[?25l";

int losgehts = 0;

CKernel *CKernel::s_pThis = 0;

CKernel::CKernel (void)
:	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
        m_Timer (&m_Interrupt),
	m_Logger (m_Options.GetLogLevel ()),
        m_USBHCI (&m_Interrupt, &m_Timer),
        m_EMMC (&m_Interrupt, &m_Timer, &m_ActLED),
	m_nPosX (0),
	m_nPosY (0)
{
	s_pThis = this;

	m_ActLED.Blink (5);	// show we are alive
}

CKernel::~CKernel (void)
{
	s_pThis = 0;
}

CKernel *CKernel::Get (void)
{
        return s_pThis;
}

boolean CKernel::Initialize (void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Screen.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Serial.Initialize (115200);
	}

	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice (m_Options.GetLogDevice (), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		bOK = m_Logger.Initialize (pTarget);
	}

	if (bOK)
	{
		bOK = m_Interrupt.Initialize ();
	}

	if (bOK)
	{
		bOK = m_Timer.Initialize ();
	}

	if (bOK)
	{
		bOK = m_USBHCI.Initialize ();
	}

        if (bOK)
        {
                bOK = m_EMMC.Initialize ();
        }

	losgehts = 1;
	return bOK;
}

void CKernel::smalltalk (void)
{
	struct options vm_options;

	vm_options.snapshot_name = "snapshot.im";
	vm_options.root_directory = "/";
	vm_options.three_buttons = true;
	vm_options.vsync = false;
	vm_options.novsync_delay = 0;  // Try -delay 8 arg if your CPU is unhappy
	vm_options.cycles_per_frame = 1800;
	vm_options.display_scale = 1;

	VirtualMachine *vm = new VirtualMachine(vm_options, m_Screen);

        m_Logger.Write (FromKernel, LogDebug, "VM init");
	if (vm->init())
	{
                m_Logger.Write (FromKernel, LogDebug, "VM run");
		vm->run();
	}
	else
	{
		m_Logger.Write (FromKernel, LogError, "VM failed to initialize (invalid/missing directory or snapshot?)");
	}
	delete vm;
}

TShutdownMode CKernel::Run (void)
{
	m_Logger.Write (FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

        CUSBKeyboardDevice *pKeyboard = (CUSBKeyboardDevice *) m_DeviceNameService.GetDevice ("ukbd1", FALSE);
        if (pKeyboard == 0)
        {
                m_Logger.Write (FromKernel, LogError, "Keyboard not found");

                return ShutdownHalt;
        }

        // pKeyboard->RegisterKeyPressedHandler (KeyPressedHandler);
        pKeyboard->RegisterKeyStatusHandlerRaw (KeyStatusHandlerRawStub);

	CMouseDevice *pMouse = (CMouseDevice *) m_DeviceNameService.GetDevice ("mouse1", FALSE);
	if (pMouse == 0)
	{
		m_Logger.Write (FromKernel, LogPanic, "Mouse not found");
	}
        m_pMouse = pMouse;

	m_Screen.Write (ClearScreen, sizeof ClearScreen-1);

	if (!pMouse->Setup (m_Screen.GetWidth (), m_Screen.GetHeight ()))
	{
		m_Logger.Write (FromKernel, LogPanic, "Cannot setup mouse");
	}

	m_nPosX = m_Screen.GetWidth () / 2;
	m_nPosY = m_Screen.GetHeight () / 2;

//	pMouse->SetCursor (m_nPosX, m_nPosY);
	pMouse->ShowCursor (FALSE);

	pMouse->RegisterEventHandler (MouseEventStub);

        CLogger::Get ()->Write ("kernel", LogDebug, "SD mount");

	FATFS m_FileSystem;

        if (f_mount (&m_FileSystem, "SD:", 1) != FR_OK)
        {
                CLogger::Get ()->Write ("ObjectMemory", LogDebug, "Cannot mount drive: %s", "sd:");
        }

	m_Logger.Write (FromKernel, LogDebug, "Starting smalltalk");
	smalltalk();
	m_Logger.Write (FromKernel, LogDebug, "Ending smalltalk");

#if 1
	for (unsigned nCount = 0; m_ShutdownMode == ShutdownNone; nCount++)
	{
		pMouse->UpdateCursor ();

		m_Screen.Rotor (0, nCount);
	}
#endif

	return m_ShutdownMode;
}

void CKernel::KeyPressedHandler (const char *pString)
{
        assert (s_pThis != 0);
        s_pThis->m_Screen.Write (pString, strlen (pString));
        s_pThis->m_Logger.Write (FromKernel, LogDebug, "Keyboard: %s", pString);
}


void CKernel::KeyStatusHandlerRawStub (unsigned char ucModifiers, const unsigned char RawKeys[6])
{
        assert (s_pThis != 0);
        s_pThis->KeyStatusHandlerRaw (ucModifiers, RawKeys);
}

void CKernel::KeyStatusHandlerRaw (unsigned char ucModifiers, const unsigned char RawKeys[6])
{
        assert (s_pThis != 0);

        CString Message;
        Message.Format ("Key status (modifiers %02X)", (unsigned) ucModifiers);

        for (unsigned i = 0; i < 6; i++)
        {
                if (RawKeys[i] != 0)
                {
                        CString KeyCode;
                        KeyCode.Format (" %02X", (unsigned) RawKeys[i]);
                        Message.Append (KeyCode);
                        m_RawKeys[i] = RawKeys[i] | ((ucModifiers & 0xff) << 8);
                }
        }

//        s_pThis->m_Logger.Write (FromKernel, LogNotice, Message);
}

void CKernel::MouseEventHandler (TMouseEvent Event, unsigned nButtons, unsigned nPosX, unsigned nPosY)
{
		m_nPosX = nPosX;
		m_nPosY = nPosY;

	switch (Event) {
    		case MouseEventMouseDown:
			if (nButtons & MOUSE_BUTTON_LEFT)
				m_nButtons = m_nButtons | 0x01;
			if (nButtons & MOUSE_BUTTON_MIDDLE)
				m_nButtons = m_nButtons | 0x02;
			if (nButtons & MOUSE_BUTTON_RIGHT)
				m_nButtons = m_nButtons | 0x04;
			break;
		case MouseEventMouseUp:
			if (nButtons & MOUSE_BUTTON_LEFT)
				m_nButtons = m_nButtons & ~0x01;
			if (nButtons & MOUSE_BUTTON_MIDDLE)
				m_nButtons = m_nButtons & ~0x02;
			if (nButtons & MOUSE_BUTTON_RIGHT)
				m_nButtons = m_nButtons & ~0x04;
			break;
        }
}

void CKernel::MouseEventStub (TMouseEvent Event, unsigned nButtons, unsigned nPosX, unsigned nPosY)
{
	assert (s_pThis != 0);
	s_pThis->MouseEventHandler (Event, nButtons, nPosX, nPosY);
}

void CKernel::DrawLine (int nPosX1, int nPosY1, int nPosX2, int nPosY2, TScreenColor Color)
{
	// Bresenham algorithm

	int nDeltaX = nPosX2-nPosX1 >= 0 ? nPosX2-nPosX1 : nPosX1-nPosX2;
	int nSignX  = nPosX1 < nPosX2 ? 1 : -1;

	int nDeltaY = -(nPosY2-nPosY1 >= 0 ? nPosY2-nPosY1 : nPosY1-nPosY2);
	int nSignY  = nPosY1 < nPosY2 ? 1 : -1;

	int nError = nDeltaX + nDeltaY;

	while (1)
	{
		m_Screen.SetPixel ((unsigned) nPosX1, (unsigned) nPosY1, Color);

		if (   nPosX1 == nPosX2
		    && nPosY1 == nPosY2)
		{
			break;
		}

		int nError2 = nError + nError;

		if (nError2 > nDeltaY)
		{
			nError += nDeltaY;
			nPosX1 += nSignX;
		}

		if (nError2 < nDeltaX)
		{
			nError += nDeltaX;
			nPosY1 += nSignY;
		}
	}
}

CMouseDevice *CKernel::GetMouseDevice (void) {
   return m_pMouse;
}

void CKernel::GetMouseState (int *x, int *y, unsigned *buttons) {
   *x = m_nPosX;
   *y = m_nPosY;
   *buttons = m_nButtons;
}

unsigned CKernel::GetTicks (void) {
	unsigned t = m_Timer.GetClockTicks() / 1000;
	return t;
}

int CKernel::GetKeyboardState (unsigned *keys) {
	int n = 0;
        for (unsigned i = 0; i < 6; i++)
        {
                if (m_RawKeys[i] != 0)
                {
                        *keys = m_RawKeys[i];
			keys++;
			n++;
                        m_RawKeys[i] = 0;
                }
        }
	return n;
}

void CKernel::SetMouseState (int x, int y) {
   m_nPosX = x;
   m_nPosY = y;
}

