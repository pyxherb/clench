[BITS 64]
;
; rdx - src
; rcx - srcSize
; r8 - bufferOut
;
.global _clcghalConvertTextureRGB8ToRGBA8

_clcghalConvertTextureRGB8ToRGBA8:
RGB8ToRGBA8_move:
	mov ax, [rdx]
	shl eax, 16
	mov ah, [rdx + 2]
	mov al, 0xff

	mov [r8], eax

	add r8, 4
	add rdx, 3

	test rcx, 0xffffffffffffffff
	jz RGB8ToRGBA8_finished

	sub rcx, 3
	jmp RGB8ToRGBA8_move

RGB8ToRGBA8_finished:
	ret
