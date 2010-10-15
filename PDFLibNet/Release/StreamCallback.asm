; Listing generated by Microsoft (R) Optimizing Compiler Version 15.00.30729.01 

	TITLE	c:\projects\PDFLibNet\libAFPDFLib\StreamCallback.cpp
	.686P
	.XMM
	include listing.inc
	.model	flat


PUBLIC	?moveStart@StreamCallback@@UAEXH@Z		; StreamCallback::moveStart
PUBLIC	?makeSubStream@StreamCallback@@UAEPAVStream@@IHIPAVObject@@@Z ; StreamCallback::makeSubStream
PUBLIC	?setPos@StreamCallback@@UAEXIH@Z		; StreamCallback::setPos
PUBLIC	?close@StreamCallback@@UAEXXZ			; StreamCallback::close
PUBLIC	?reset@StreamCallback@@UAEXXZ			; StreamCallback::reset
PUBLIC	??_R4StreamCallback@@6B@			; StreamCallback::`RTTI Complete Object Locator'
PUBLIC	??_R3StreamCallback@@8				; StreamCallback::`RTTI Class Hierarchy Descriptor'
PUBLIC	??_R2StreamCallback@@8				; StreamCallback::`RTTI Base Class Array'
PUBLIC	??_R1A@?0A@EA@BaseStream@@8			; BaseStream::`RTTI Base Class Descriptor at (0,-1,0,64)'
PUBLIC	??_R3BaseStream@@8				; BaseStream::`RTTI Class Hierarchy Descriptor'
PUBLIC	??_R2BaseStream@@8				; BaseStream::`RTTI Base Class Array'
PUBLIC	??_R0?AVBaseStream@@@8				; BaseStream `RTTI Type Descriptor'
PUBLIC	??_R1A@?0A@EA@StreamCallback@@8			; StreamCallback::`RTTI Base Class Descriptor at (0,-1,0,64)'
PUBLIC	??_R0?AVStreamCallback@@@8			; StreamCallback `RTTI Type Descriptor'
PUBLIC	?isEncoder@Stream@@UAEHXZ			; Stream::isEncoder
PUBLIC	?getNextStream@Stream@@UAEPAV1@XZ		; Stream::getNextStream
PUBLIC	?isBinary@BaseStream@@UAEHH@Z			; BaseStream::isBinary
PUBLIC	?getBaseStream@BaseStream@@UAEPAV1@XZ		; BaseStream::getBaseStream
PUBLIC	?getUndecodedStream@BaseStream@@UAEPAVStream@@XZ ; BaseStream::getUndecodedStream
PUBLIC	?getDict@BaseStream@@UAEPAVDict@@XZ		; BaseStream::getDict
PUBLIC	?getFileName@BaseStream@@UAEPAVGString@@XZ	; BaseStream::getFileName
PUBLIC	?getKind@StreamCallback@@UAE?AW4StreamKind@@XZ	; StreamCallback::getKind
PUBLIC	?getChar@StreamCallback@@UAEHXZ			; StreamCallback::getChar
PUBLIC	?lookChar@StreamCallback@@UAEHXZ		; StreamCallback::lookChar
PUBLIC	?getPos@StreamCallback@@UAEHXZ			; StreamCallback::getPos
PUBLIC	?getStart@StreamCallback@@UAEIXZ		; StreamCallback::getStart
PUBLIC	?getLength@StreamCallback@@UAEHXZ		; StreamCallback::getLength
PUBLIC	??_GStreamCallback@@UAEPAXI@Z			; StreamCallback::`scalar deleting destructor'
PUBLIC	??_7StreamCallback@@6B@				; StreamCallback::`vftable'
EXTRN	??_EStreamCallback@@UAEPAXI@Z:PROC		; StreamCallback::`vector deleting destructor'
;	COMDAT ??_7StreamCallback@@6B@
CONST	SEGMENT
??_7StreamCallback@@6B@ DD FLAT:??_R4StreamCallback@@6B@ ; StreamCallback::`vftable'
	DD	FLAT:??_EStreamCallback@@UAEPAXI@Z
	DD	FLAT:?getKind@StreamCallback@@UAE?AW4StreamKind@@XZ
	DD	FLAT:?reset@StreamCallback@@UAEXXZ
	DD	FLAT:?close@StreamCallback@@UAEXXZ
	DD	FLAT:?getChar@StreamCallback@@UAEHXZ
	DD	FLAT:?lookChar@StreamCallback@@UAEHXZ
	DD	FLAT:?getRawChar@Stream@@UAEHXZ
	DD	FLAT:?getLine@Stream@@UAEPADPADH@Z
	DD	FLAT:?getPos@StreamCallback@@UAEHXZ
	DD	FLAT:?setPos@StreamCallback@@UAEXIH@Z
	DD	FLAT:?getPSFilter@Stream@@UAEPAVGString@@HPAD@Z
	DD	FLAT:?isBinary@BaseStream@@UAEHH@Z
	DD	FLAT:?getBaseStream@BaseStream@@UAEPAV1@XZ
	DD	FLAT:?getUndecodedStream@BaseStream@@UAEPAVStream@@XZ
	DD	FLAT:?getDict@BaseStream@@UAEPAVDict@@XZ
	DD	FLAT:?isEncoder@Stream@@UAEHXZ
	DD	FLAT:?getImageParams@Stream@@UAEXPAHPAW4StreamColorSpaceMode@@@Z
	DD	FLAT:?getNextStream@Stream@@UAEPAV1@XZ
	DD	FLAT:?makeSubStream@StreamCallback@@UAEPAVStream@@IHIPAVObject@@@Z
	DD	FLAT:?getFileName@BaseStream@@UAEPAVGString@@XZ
	DD	FLAT:?getStart@StreamCallback@@UAEIXZ
	DD	FLAT:?moveStart@StreamCallback@@UAEXH@Z
	DD	FLAT:?getLength@StreamCallback@@UAEHXZ
CONST	ENDS
;	COMDAT ??_R4StreamCallback@@6B@
rdata$r	SEGMENT
??_R4StreamCallback@@6B@ DD 00H				; StreamCallback::`RTTI Complete Object Locator'
	DD	00H
	DD	00H
	DD	FLAT:??_R0?AVStreamCallback@@@8
	DD	FLAT:??_R3StreamCallback@@8
rdata$r	ENDS
;	COMDAT ??_R3StreamCallback@@8
rdata$r	SEGMENT
??_R3StreamCallback@@8 DD 00H				; StreamCallback::`RTTI Class Hierarchy Descriptor'
	DD	00H
	DD	03H
	DD	FLAT:??_R2StreamCallback@@8
rdata$r	ENDS
;	COMDAT ??_R2StreamCallback@@8
rdata$r	SEGMENT
??_R2StreamCallback@@8 DD FLAT:??_R1A@?0A@EA@StreamCallback@@8 ; StreamCallback::`RTTI Base Class Array'
	DD	FLAT:??_R1A@?0A@EA@BaseStream@@8
	DD	FLAT:??_R1A@?0A@EA@Stream@@8
rdata$r	ENDS
;	COMDAT ??_R1A@?0A@EA@BaseStream@@8
rdata$r	SEGMENT
??_R1A@?0A@EA@BaseStream@@8 DD FLAT:??_R0?AVBaseStream@@@8 ; BaseStream::`RTTI Base Class Descriptor at (0,-1,0,64)'
	DD	01H
	DD	00H
	DD	0ffffffffH
	DD	00H
	DD	040H
	DD	FLAT:??_R3BaseStream@@8
rdata$r	ENDS
;	COMDAT ??_R3BaseStream@@8
rdata$r	SEGMENT
??_R3BaseStream@@8 DD 00H				; BaseStream::`RTTI Class Hierarchy Descriptor'
	DD	00H
	DD	02H
	DD	FLAT:??_R2BaseStream@@8
rdata$r	ENDS
;	COMDAT ??_R2BaseStream@@8
rdata$r	SEGMENT
??_R2BaseStream@@8 DD FLAT:??_R1A@?0A@EA@BaseStream@@8	; BaseStream::`RTTI Base Class Array'
	DD	FLAT:??_R1A@?0A@EA@Stream@@8
rdata$r	ENDS
;	COMDAT ??_R0?AVBaseStream@@@8
_DATA	SEGMENT
??_R0?AVBaseStream@@@8 DD FLAT:??_7type_info@@6B@	; BaseStream `RTTI Type Descriptor'
	DD	00H
	DB	'.?AVBaseStream@@', 00H
_DATA	ENDS
;	COMDAT ??_R1A@?0A@EA@StreamCallback@@8
rdata$r	SEGMENT
??_R1A@?0A@EA@StreamCallback@@8 DD FLAT:??_R0?AVStreamCallback@@@8 ; StreamCallback::`RTTI Base Class Descriptor at (0,-1,0,64)'
	DD	02H
	DD	00H
	DD	0ffffffffH
	DD	00H
	DD	040H
	DD	FLAT:??_R3StreamCallback@@8
rdata$r	ENDS
;	COMDAT ??_R0?AVStreamCallback@@@8
_DATA	SEGMENT
??_R0?AVStreamCallback@@@8 DD FLAT:??_7type_info@@6B@	; StreamCallback `RTTI Type Descriptor'
	DD	00H
	DB	'.?AVStreamCallback@@', 00H
	ORG $+7572
__bad_alloc_Message DD FLAT:??_C@_0P@GHFPNOJB@bad?5allocation?$AA@
; Function compile flags: /Ogtpy
; File c:\projects\pdflibnet\libafpdflib\streamcallback.cpp
;	COMDAT ?moveStart@StreamCallback@@UAEXH@Z
_TEXT	SEGMENT
_delta$ = 8						; size = 4
?moveStart@StreamCallback@@UAEXH@Z PROC			; StreamCallback::moveStart, COMDAT
; _this$ = ecx

; 96   :   start += delta;

	mov	eax, DWORD PTR _delta$[esp-4]
	add	DWORD PTR [ecx+40], eax
	mov	edx, DWORD PTR [ecx+40]

; 97   :   bufPtr = bufEnd = buf;

	lea	eax, DWORD PTR [ecx+52]
	mov	DWORD PTR [ecx+8248], eax
	mov	DWORD PTR [ecx+8244], eax

; 98   :   bufPos = start;

	mov	DWORD PTR [ecx+8252], edx

; 99   : }

	ret	4
?moveStart@StreamCallback@@UAEXH@Z ENDP			; StreamCallback::moveStart
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?setPos@StreamCallback@@UAEXIH@Z
_TEXT	SEGMENT
_pos$ = 8						; size = 4
_dir$ = 12						; size = 4
?setPos@StreamCallback@@UAEXIH@Z PROC			; StreamCallback::setPos, COMDAT
; _this$ = ecx

; 81   :   if (dir >= 0) {

	cmp	DWORD PTR _dir$[esp-4], 0
	jl	SHORT $LN3@setPos

; 82   : 	currentPos = pos;

	mov	eax, DWORD PTR _pos$[esp-4]

; 83   :     bufPos = pos;
; 84   :   } else {

	jmp	SHORT $LN6@setPos
$LN3@setPos:

; 85   : 	
; 86   :     if (pos > _fullLenght)

	mov	eax, DWORD PTR [ecx+36]
	mov	edx, DWORD PTR _pos$[esp-4]
	cmp	edx, eax
	jbe	SHORT $LN1@setPos

; 87   :       pos = (Guint)_fullLenght;

	mov	edx, eax
$LN1@setPos:

; 88   :     
; 89   :     bufPos = _fullLenght-pos;

	sub	eax, edx
$LN6@setPos:

; 90   : 	currentPos = bufPos;

	mov	DWORD PTR [ecx+8260], eax
	mov	DWORD PTR [ecx+8252], eax

; 91   :   }
; 92   :   bufPtr = bufEnd = buf;

	lea	eax, DWORD PTR [ecx+52]
	mov	DWORD PTR [ecx+8248], eax
	mov	DWORD PTR [ecx+8244], eax

; 93   : }

	ret	8
?setPos@StreamCallback@@UAEXIH@Z ENDP			; StreamCallback::setPos
_TEXT	ENDS
PUBLIC	?fillBuf@StreamCallback@@AAEHXZ			; StreamCallback::fillBuf
EXTRN	__except_handler4:PROC
EXTRN	__local_unwind4:PROC
;	COMDAT xdata$x
xdata$x	SEGMENT
__sehtable$?fillBuf@StreamCallback@@AAEHXZ DD 0fffffffeH
	DD	00H
	DD	0ffffffd8H
	DD	00H
	DD	0fffffffeH
	DD	00H
	DD	FLAT:$LN11@fillBuf
; Function compile flags: /Ogtpy
xdata$x	ENDS
;	COMDAT ?fillBuf@StreamCallback@@AAEHXZ
_TEXT	SEGMENT
__$SEHRec$ = -24					; size = 24
?fillBuf@StreamCallback@@AAEHXZ PROC			; StreamCallback::fillBuf, COMDAT
; _this$ = ecx

; 46   : GBool StreamCallback::fillBuf() {

	push	ebp
	mov	ebp, esp
	push	-2					; fffffffeH
	push	OFFSET __sehtable$?fillBuf@StreamCallback@@AAEHXZ
	push	OFFSET __except_handler4
	mov	eax, DWORD PTR fs:0
	push	eax
	sub	esp, 8
	push	ebx
	push	esi
	push	edi
	mov	eax, DWORD PTR ___security_cookie
	xor	DWORD PTR __$SEHRec$[ebp+16], eax
	xor	eax, ebp
	push	eax
	lea	eax, DWORD PTR __$SEHRec$[ebp+8]
	mov	DWORD PTR fs:0, eax
	mov	esi, ecx

; 47   :   int n;
; 48   :   gLockMutex(&gStreamMutex);

	push	OFFSET ?gStreamMutex@@3U_RTL_CRITICAL_SECTION@@A ; gStreamMutex
	call	DWORD PTR __imp__EnterCriticalSection@4

; 49   :   __try { 	  

	mov	DWORD PTR __$SEHRec$[ebp+20], 0

; 50   : 	  if(_callback==0)

	cmp	DWORD PTR [esi+32], 0
	jne	SHORT $LN6@fillBuf
$LN17@fillBuf:

; 51   : 		  return gFalse;

	push	-2					; fffffffeH
$LN18@fillBuf:
	lea	eax, DWORD PTR __$SEHRec$[ebp+8]
	push	eax
	push	OFFSET ___security_cookie
	call	__local_unwind4
	add	esp, 12					; 0000000cH
	xor	eax, eax
$LN14@fillBuf:

; 76   :   }
; 77   : }

	mov	ecx, DWORD PTR __$SEHRec$[ebp+8]
	mov	DWORD PTR fs:0, ecx
	pop	ecx
	pop	edi
	pop	esi
	pop	ebx
	mov	esp, ebp
	pop	ebp
	ret	0
$LN6@fillBuf:

; 52   : 	  
; 53   : 	  bufPos += bufEnd - buf;

	mov	ecx, DWORD PTR [esi+8248]
	sub	ecx, esi
	sub	ecx, 52					; 00000034H
	add	DWORD PTR [esi+8252], ecx

; 54   : 	  bufPtr = bufEnd = buf;

	lea	eax, DWORD PTR [esi+52]
	mov	DWORD PTR [esi+8248], eax
	mov	DWORD PTR [esi+8244], eax

; 55   : 	  if (limited && bufPos >= start + length) 

	mov	eax, DWORD PTR [esi+44]
	test	eax, eax
	je	SHORT $LN4@fillBuf
	mov	edx, DWORD PTR [esi+48]
	add	edx, DWORD PTR [esi+40]
	cmp	DWORD PTR [esi+8252], edx

; 56   : 		return gFalse;

	jae	SHORT $LN17@fillBuf

; 57   : 
; 58   : 	  if (limited && bufPos + fileStreamBufSize > start + length) {

	test	eax, eax
	je	SHORT $LN4@fillBuf
	mov	eax, DWORD PTR [esi+48]
	mov	edi, DWORD PTR [esi+40]
	mov	ecx, DWORD PTR [esi+8252]
	lea	edx, DWORD PTR [edi+eax]
	lea	ebx, DWORD PTR [ecx+8192]
	cmp	ebx, edx
	jbe	SHORT $LN4@fillBuf

; 59   : 		n = start + length - bufPos;

	sub	edi, ecx
	add	edi, eax

; 60   : 	  } else {

	jmp	SHORT $LN3@fillBuf
$LN4@fillBuf:

; 61   : 		n = fileStreamBufSize;

	mov	edi, 8192				; 00002000H
$LN3@fillBuf:

; 62   : 	  }
; 63   : 	  if(bufPos + n > _fullLenght)

	mov	eax, DWORD PTR [esi+8252]
	mov	ecx, DWORD PTR [esi+36]
	lea	edx, DWORD PTR [eax+edi]
	cmp	edx, ecx
	jbe	SHORT $LN2@fillBuf

; 64   : 		  n = _fullLenght - bufPos;

	sub	ecx, eax
	mov	edi, ecx
$LN2@fillBuf:

; 65   : 	  _callback((unsigned char *)buf,1,bufPos,n);

	push	edi
	push	eax
	push	1
	lea	eax, DWORD PTR [esi+52]
	push	eax
	call	DWORD PTR [esi+32]

; 66   : 	  //n = fread(buf, 1, n, f);
; 67   : 	  bufEnd = buf + n;

	lea	eax, DWORD PTR [edi+esi+52]
	mov	DWORD PTR [esi+8248], eax

; 69   : 		return gFalse;

	push	-2					; fffffffeH

; 68   : 	  if (bufPtr >= bufEnd) 

	cmp	DWORD PTR [esi+8244], eax

; 69   : 		return gFalse;

	jae	$LN18@fillBuf

; 70   : 	  
; 71   : 	  return gTrue;

	lea	ecx, DWORD PTR __$SEHRec$[ebp+8]
	push	ecx
	push	OFFSET ___security_cookie
	call	__local_unwind4
	add	esp, 12					; 0000000cH
	mov	eax, 1

; 76   :   }
; 77   : }

	mov	ecx, DWORD PTR __$SEHRec$[ebp+8]
	mov	DWORD PTR fs:0, ecx
	pop	ecx
	pop	edi
	pop	esi
	pop	ebx
	mov	esp, ebp
	pop	ebp
	ret	0
$LN11@fillBuf:
$LN19@fillBuf:
$LN13@fillBuf:

; 72   :   }
; 73   :   __finally 
; 74   :   {
; 75   : 	  gUnlockMutex(&gStreamMutex);

	push	OFFSET ?gStreamMutex@@3U_RTL_CRITICAL_SECTION@@A ; gStreamMutex
	call	DWORD PTR __imp__LeaveCriticalSection@4
$LN12@fillBuf:
$LN15@fillBuf:
	ret	0
?fillBuf@StreamCallback@@AAEHXZ ENDP			; StreamCallback::fillBuf
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?close@StreamCallback@@UAEXXZ
_TEXT	SEGMENT
?close@StreamCallback@@UAEXXZ PROC			; StreamCallback::close, COMDAT
; _this$ = ecx

; 40   :   if (saved) {

	cmp	DWORD PTR [ecx+8264], 0
	je	SHORT $LN1@close

; 41   : 	currentPos=savePos;	

	mov	eax, DWORD PTR [ecx+8256]
	mov	DWORD PTR [ecx+8260], eax

; 42   :     saved = gFalse;

	mov	DWORD PTR [ecx+8264], 0
$LN1@close:

; 43   :   }
; 44   : }

	ret	0
?close@StreamCallback@@UAEXXZ ENDP			; StreamCallback::close
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?reset@StreamCallback@@UAEXXZ
_TEXT	SEGMENT
?reset@StreamCallback@@UAEXXZ PROC			; StreamCallback::reset, COMDAT
; _this$ = ecx

; 30   :   savePos = currentPos+start;

	mov	eax, DWORD PTR [ecx+40]
	mov	edx, DWORD PTR [ecx+8260]
	add	edx, eax
	mov	DWORD PTR [ecx+8256], edx

; 31   :   currentPos =start;
; 32   : 
; 33   :   saved = gTrue;
; 34   :   bufPtr = bufEnd = buf;

	lea	edx, DWORD PTR [ecx+52]
	mov	DWORD PTR [ecx+8260], eax
	mov	DWORD PTR [ecx+8264], 1
	mov	DWORD PTR [ecx+8248], edx
	mov	DWORD PTR [ecx+8244], edx

; 35   :   bufPos = start;

	mov	DWORD PTR [ecx+8252], eax

; 36   : }

	ret	0
?reset@StreamCallback@@UAEXXZ ENDP			; StreamCallback::reset
; Function compile flags: /Ogtpy
; File c:\projects\pdflibnet\libafpdflib\streamcallback.h
_TEXT	ENDS
;	COMDAT ?getLength@StreamCallback@@UAEHXZ
_TEXT	SEGMENT
?getLength@StreamCallback@@UAEHXZ PROC			; StreamCallback::getLength, COMDAT
; _this$ = ecx

; 28   :   virtual int getLength(){ return length; } 

	mov	eax, DWORD PTR [ecx+48]
	ret	0
?getLength@StreamCallback@@UAEHXZ ENDP			; StreamCallback::getLength
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?getStart@StreamCallback@@UAEIXZ
_TEXT	SEGMENT
?getStart@StreamCallback@@UAEIXZ PROC			; StreamCallback::getStart, COMDAT
; _this$ = ecx

; 26   :   virtual Guint getStart() { return start; }

	mov	eax, DWORD PTR [ecx+40]
	ret	0
?getStart@StreamCallback@@UAEIXZ ENDP			; StreamCallback::getStart
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?getPos@StreamCallback@@UAEHXZ
_TEXT	SEGMENT
?getPos@StreamCallback@@UAEHXZ PROC			; StreamCallback::getPos, COMDAT
; _this$ = ecx

; 24   :   virtual int getPos() { return bufPos + (bufPtr - buf); }

	mov	eax, DWORD PTR [ecx+8252]
	add	eax, DWORD PTR [ecx+8244]
	sub	eax, ecx
	sub	eax, 52					; 00000034H
	ret	0
?getPos@StreamCallback@@UAEHXZ ENDP			; StreamCallback::getPos
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?lookChar@StreamCallback@@UAEHXZ
_TEXT	SEGMENT
?lookChar@StreamCallback@@UAEHXZ PROC			; StreamCallback::lookChar, COMDAT
; _this$ = ecx

; 23   :     { return (bufPtr >= bufEnd && !fillBuf()) ? EOF : (*bufPtr & 0xff); }

	push	esi
	mov	esi, ecx
	mov	eax, DWORD PTR [esi+8244]
	cmp	eax, DWORD PTR [esi+8248]
	jb	SHORT $LN3@lookChar
	call	?fillBuf@StreamCallback@@AAEHXZ		; StreamCallback::fillBuf
	test	eax, eax
	jne	SHORT $LN3@lookChar
	or	eax, -1
	pop	esi
	ret	0
$LN3@lookChar:
	mov	ecx, DWORD PTR [esi+8244]
	movzx	eax, BYTE PTR [ecx]
	pop	esi
	ret	0
?lookChar@StreamCallback@@UAEHXZ ENDP			; StreamCallback::lookChar
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?getChar@StreamCallback@@UAEHXZ
_TEXT	SEGMENT
?getChar@StreamCallback@@UAEHXZ PROC			; StreamCallback::getChar, COMDAT
; _this$ = ecx

; 21   :     { return (bufPtr >= bufEnd && !fillBuf()) ? EOF : (*bufPtr++ & 0xff); }

	push	esi
	mov	esi, ecx
	mov	eax, DWORD PTR [esi+8244]
	cmp	eax, DWORD PTR [esi+8248]
	jb	SHORT $LN3@getChar
	call	?fillBuf@StreamCallback@@AAEHXZ		; StreamCallback::fillBuf
	test	eax, eax
	jne	SHORT $LN3@getChar
	or	eax, -1
	pop	esi
	ret	0
$LN3@getChar:
	mov	ecx, DWORD PTR [esi+8244]
	movzx	eax, BYTE PTR [ecx]
	inc	ecx
	mov	DWORD PTR [esi+8244], ecx
	pop	esi
	ret	0
?getChar@StreamCallback@@UAEHXZ ENDP			; StreamCallback::getChar
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?getKind@StreamCallback@@UAE?AW4StreamKind@@XZ
_TEXT	SEGMENT
?getKind@StreamCallback@@UAE?AW4StreamKind@@XZ PROC	; StreamCallback::getKind, COMDAT
; _this$ = ecx

; 17   :   virtual StreamKind getKind() { return strFile; }

	xor	eax, eax
	ret	0
?getKind@StreamCallback@@UAE?AW4StreamKind@@XZ ENDP	; StreamCallback::getKind
; Function compile flags: /Ogtpy
; File c:\projects\pdflibnet\xpdfwin\xpdf\stream.h
_TEXT	ENDS
;	COMDAT ?getFileName@BaseStream@@UAEPAVGString@@XZ
_TEXT	SEGMENT
?getFileName@BaseStream@@UAEPAVGString@@XZ PROC		; BaseStream::getFileName, COMDAT
; _this$ = ecx

; 158  :   virtual GString *getFileName() { return NULL; }

	xor	eax, eax
	ret	0
?getFileName@BaseStream@@UAEPAVGString@@XZ ENDP		; BaseStream::getFileName
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?getUndecodedStream@BaseStream@@UAEPAVStream@@XZ
_TEXT	SEGMENT
?getUndecodedStream@BaseStream@@UAEPAVStream@@XZ PROC	; BaseStream::getUndecodedStream, COMDAT
; _this$ = ecx

; 156  :   virtual Stream *getUndecodedStream() { return this; }

	mov	eax, ecx
	ret	0
?getUndecodedStream@BaseStream@@UAEPAVStream@@XZ ENDP	; BaseStream::getUndecodedStream
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?getBaseStream@BaseStream@@UAEPAV1@XZ
_TEXT	SEGMENT
?getBaseStream@BaseStream@@UAEPAV1@XZ PROC		; BaseStream::getBaseStream, COMDAT
; _this$ = ecx

; 155  :   virtual BaseStream *getBaseStream() { return this; }

	mov	eax, ecx
	ret	0
?getBaseStream@BaseStream@@UAEPAV1@XZ ENDP		; BaseStream::getBaseStream
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?isBinary@BaseStream@@UAEHH@Z
_TEXT	SEGMENT
_last$ = 8						; size = 4
?isBinary@BaseStream@@UAEHH@Z PROC			; BaseStream::isBinary, COMDAT
; _this$ = ecx

; 154  :   virtual GBool isBinary(GBool last = gTrue) { return last; }

	mov	eax, DWORD PTR _last$[esp-4]
	ret	4
?isBinary@BaseStream@@UAEHH@Z ENDP			; BaseStream::isBinary
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?getNextStream@Stream@@UAEPAV1@XZ
_TEXT	SEGMENT
?getNextStream@Stream@@UAEPAV1@XZ PROC			; Stream::getNextStream, COMDAT
; _this$ = ecx

; 127  :   virtual Stream *getNextStream() { return NULL; }

	xor	eax, eax
	ret	0
?getNextStream@Stream@@UAEPAV1@XZ ENDP			; Stream::getNextStream
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ?isEncoder@Stream@@UAEHXZ
_TEXT	SEGMENT
?isEncoder@Stream@@UAEHXZ PROC				; Stream::isEncoder, COMDAT
; _this$ = ecx

; 120  :   virtual GBool isEncoder() { return gFalse; }

	xor	eax, eax
	ret	0
?isEncoder@Stream@@UAEHXZ ENDP				; Stream::isEncoder
; Function compile flags: /Ogtpy
;	COMDAT ?getDict@BaseStream@@UAEPAVDict@@XZ
_TEXT	SEGMENT
?getDict@BaseStream@@UAEPAVDict@@XZ PROC		; BaseStream::getDict, COMDAT
; _this$ = ecx

; 157  :   virtual Dict *getDict() { return dict.getDict(); }

	mov	eax, DWORD PTR [ecx+24]
	ret	0
?getDict@BaseStream@@UAEPAVDict@@XZ ENDP		; BaseStream::getDict
PUBLIC	??1StreamCallback@@UAE@XZ			; StreamCallback::~StreamCallback
; Function compile flags: /Ogtpy
; File c:\projects\pdflibnet\libafpdflib\streamcallback.cpp
;	COMDAT ??1StreamCallback@@UAE@XZ
_TEXT	SEGMENT
??1StreamCallback@@UAE@XZ PROC				; StreamCallback::~StreamCallback, COMDAT
; _this$ = ecx

; 20   :   close();  

	cmp	DWORD PTR [ecx+8264], 0
	mov	DWORD PTR [ecx], OFFSET ??_7StreamCallback@@6B@
	je	SHORT $LN4@StreamCall
	mov	eax, DWORD PTR [ecx+8256]
	mov	DWORD PTR [ecx+8260], eax
	mov	DWORD PTR [ecx+8264], 0
$LN4@StreamCall:

; 21   : }

	jmp	??1BaseStream@@UAE@XZ			; BaseStream::~BaseStream
??1StreamCallback@@UAE@XZ ENDP				; StreamCallback::~StreamCallback
; Function compile flags: /Ogtpy
_TEXT	ENDS
;	COMDAT ??_GStreamCallback@@UAEPAXI@Z
_TEXT	SEGMENT
___flags$ = 8						; size = 4
??_GStreamCallback@@UAEPAXI@Z PROC			; StreamCallback::`scalar deleting destructor', COMDAT
; _this$ = ecx
	push	esi
	mov	esi, ecx
	cmp	DWORD PTR [esi+8264], 0
	mov	DWORD PTR [esi], OFFSET ??_7StreamCallback@@6B@
	je	SHORT $LN7@scalar@10
	mov	eax, DWORD PTR [esi+8256]
	mov	DWORD PTR [esi+8260], eax
	mov	DWORD PTR [esi+8264], 0
$LN7@scalar@10:
	call	??1BaseStream@@UAE@XZ			; BaseStream::~BaseStream
	test	BYTE PTR ___flags$[esp], 1
	je	SHORT $LN10@scalar@10
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$LN10@scalar@10:
	mov	eax, esi
	pop	esi
	ret	4
??_GStreamCallback@@UAEPAXI@Z ENDP			; StreamCallback::`scalar deleting destructor'
_TEXT	ENDS
PUBLIC	??0StreamCallback@@QAE@P6GXPAEHHH@ZJIHIPAVObject@@@Z ; StreamCallback::StreamCallback
; Function compile flags: /Ogtpy
;	COMDAT ??0StreamCallback@@QAE@P6GXPAEHHH@ZJIHIPAVObject@@@Z
_TEXT	SEGMENT
_callback$ = 8						; size = 4
_fullLenght$ = 12					; size = 4
_limitedA$ = 16						; size = 4
_lengthA$ = 20						; size = 4
??0StreamCallback@@QAE@P6GXPAEHHH@ZJIHIPAVObject@@@Z PROC ; StreamCallback::StreamCallback, COMDAT
; _this$ = esi
; _startA$ = edi
; _dictA$ = eax

; 5    : {

	push	eax
	mov	ecx, esi
	call	??0BaseStream@@QAE@PAVObject@@@Z	; BaseStream::BaseStream

; 6    :   _callback = callback;
; 7    :   _fullLenght = fullLenght;
; 8    :   currentPos=startA;
; 9    :   start = startA;
; 10   :   limited = limitedA;

	mov	eax, DWORD PTR _limitedA$[esp-4]
	mov	ecx, DWORD PTR _callback$[esp-4]
	mov	edx, DWORD PTR _fullLenght$[esp-4]
	mov	DWORD PTR [esi+44], eax

; 11   :   length = lengthA;
; 12   :   bufPtr = bufEnd = buf;

	lea	eax, DWORD PTR [esi+52]
	mov	DWORD PTR [esi+8248], eax
	mov	DWORD PTR [esi+8244], eax

; 13   :   bufPos = start;
; 14   :   savePos = 0;

	xor	eax, eax
	mov	DWORD PTR [esi+32], ecx
	mov	ecx, DWORD PTR _lengthA$[esp-4]
	mov	DWORD PTR [esi+8256], eax

; 15   :   saved = gFalse;

	mov	DWORD PTR [esi+8264], eax
	mov	DWORD PTR [esi], OFFSET ??_7StreamCallback@@6B@
	mov	DWORD PTR [esi+36], edx
	mov	DWORD PTR [esi+8260], edi
	mov	DWORD PTR [esi+40], edi
	mov	DWORD PTR [esi+48], ecx
	mov	DWORD PTR [esi+8252], edi

; 16   : 
; 17   : }

	mov	eax, esi
	ret	16					; 00000010H
??0StreamCallback@@QAE@P6GXPAEHHH@ZJIHIPAVObject@@@Z ENDP ; StreamCallback::StreamCallback
; Function compile flags: /Ogtpy
;	COMDAT ?makeSubStream@StreamCallback@@UAEPAVStream@@IHIPAVObject@@@Z
_TEXT	SEGMENT
_startA$ = 8						; size = 4
_limitedA$ = 12						; size = 4
_lengthA$ = 16						; size = 4
_dictA$ = 20						; size = 4
?makeSubStream@StreamCallback@@UAEPAVStream@@IHIPAVObject@@@Z PROC ; StreamCallback::makeSubStream, COMDAT
; _this$ = ecx

; 23   : Stream *StreamCallback::makeSubStream(Guint startA, GBool limitedA, Guint lengthA, Object *dictA) {

	push	esi
	push	edi

; 24   :   return new StreamCallback(_callback,_fullLenght, startA, limitedA, lengthA, dictA);

	push	8272					; 00002050H
	mov	edi, ecx
	call	??2@YAPAXI@Z				; operator new
	mov	esi, eax
	add	esp, 4
	test	esi, esi
	je	SHORT $LN3@makeSubStr
	mov	eax, DWORD PTR _lengthA$[esp+4]
	mov	ecx, DWORD PTR _limitedA$[esp+4]
	mov	edx, DWORD PTR [edi+36]
	push	eax
	mov	eax, DWORD PTR [edi+32]
	mov	edi, DWORD PTR _startA$[esp+8]
	push	ecx
	push	edx
	push	eax
	mov	eax, DWORD PTR _dictA$[esp+20]
	call	??0StreamCallback@@QAE@P6GXPAEHHH@ZJIHIPAVObject@@@Z ; StreamCallback::StreamCallback
	pop	edi
	pop	esi

; 25   : }

	ret	16					; 00000010H
$LN3@makeSubStr:
	pop	edi

; 24   :   return new StreamCallback(_callback,_fullLenght, startA, limitedA, lengthA, dictA);

	xor	eax, eax
	pop	esi

; 25   : }

	ret	16					; 00000010H
?makeSubStream@StreamCallback@@UAEPAVStream@@IHIPAVObject@@@Z ENDP ; StreamCallback::makeSubStream
END
