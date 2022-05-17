#ifndef __TARGETDEFINES_H_
#define __TARGETDEFINES_H_

/****** some useful defaults (if not already set in the build profile) *****/

/****** Device Type *****/

#ifndef SYSTARGET_DEVICE_TYPE
	#define SYSTARGET_DEVICE_TYPE		SYSTARGET_TYPE_PROGRAMMABLE
#endif

/****** Vendor ID *****/

#ifndef SYSTARGET_VENDOR_ID
	#define SYSTARGET_VENDOR_ID			RTS_VENDORID_3S
#endif

/****** Vendor Name *****/

#ifndef SYSTARGET_VENDOR_NAME
	#define	SYSTARGET_VENDOR_NAME		"3S - Smart Software Solutions GmbH"
#endif

/****** Device Name *****/

#ifndef SYSTARGET_DEVICE_NAME
	#ifdef QNX
		#define SYSTARGET_DEVICE_NAME		RTS_PRODUCT_NAME" QNX6"
	#endif
	#ifdef LINUX
		#define SYSTARGET_DEVICE_NAME		RTS_PRODUCT_NAME" Linux"
	#endif
#endif

/****** Device Version *****/

#ifndef SYSTARGET_DEVICE_VERSION
	#define SYSTARGET_DEVICE_VERSION	RTS_VERSION
#endif

/****** Device ID *****/

#ifndef SYSTARGET_DEVICE_ID
	#if defined (TRG_X86)
		#ifdef TRG_64BIT
			#define SYSTARGET_DEVICE_ID		0x00008021
		#else
			#define SYSTARGET_DEVICE_ID		0x0000800C
		#endif
	#elif defined (TRG_ARM)
		#define SYSTARGET_DEVICE_ID		0x00008002
	#elif defined (TRG_PPC)
		#define SYSTARGET_DEVICE_ID 	0x00008008
	#elif defined (TRG_CORTEX)
		#define SYSTARGET_DEVICE_ID		0x0000800F
	#elif defined (TRG_MIPS)
		#define SYSTARGET_DEVICE_ID		0x00008003
	#elif defined (TRG_SH)
		#ifdef TRG_SH3
			#define SYSTARGET_DEVICE_ID		0x00008005
		#endif
		#ifdef TRG_SH4
			#define SYSTARGET_DEVICE_ID		0x00008006
		#endif
	#else
		#define SYSTARGET_DEVICE_ID		0
	#endif
#endif

/****** Signature *****/

#ifdef LINUX
#ifndef SYSTARGET_SIGNATUREID
	#if defined (TRG_X86)
		#ifdef TRG_64BIT
			#define SYSTARGET_SIGNATUREID		0x622Ad202
		#else
			#define SYSTARGET_SIGNATUREID		0x0A71AB63
		#endif
	#elif defined (TRG_ARM)
		#define SYSTARGET_SIGNATUREID		0x054F8ABC
	#elif defined (TRG_PPC)
		#define SYSTARGET_SIGNATUREID		0x25BE5BF6
	#elif defined (TRG_MIPS)
		#define SYSTARGET_SIGNATUREID		0xC887F9C1
	#elif defined (TRG_SH)
		#if defined (TRG_SH3)
			#define SYSTARGET_SIGNATUREID		0x69C4782B
		#elif defined (TRG_SH4)
			#define SYSTARGET_SIGNATUREID		0x668F01C5
		#endif
	#elif defined (TRG_CORTEX)
		#define SYSTARGET_SIGNATUREID		0x85DB5266
	#endif
#endif
#endif

#ifdef QNX
#ifndef SYSTARGET_SIGNATUREID
	#if defined (TRG_X86)
		#define SYSTARGET_SIGNATUREID		0xB4910F21
	#elif defined (TRG_ARM)
		#define SYSTARGET_SIGNATUREID		0xBBAF2EFE
	#elif defined (TRG_PPC)
		#define SYSTARGET_SIGNATUREID		0x9B5EFFB4
	#elif defined (TRG_MIPS)
		#define SYSTARGET_SIGNATUREID		0x76675D83
	#elif defined (TRG_SH)
		#if defined (TRG_SH3)
			#define SYSTARGET_SIGNATUREID		0xD724DC69
		#elif defined (TRG_SH4)
			#define SYSTARGET_SIGNATUREID		0xD86FA587
		#endif
	#elif defined (TRG_CORTEX)
		#define SYSTARGET_SIGNATUREID		0x3B3BF624
	#endif
#endif
#endif

#endif

