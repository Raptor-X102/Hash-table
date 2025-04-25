BITS 64
section .text
global List_find_element

List_find_element:
    push r15
    push r14
    push r13
    mov r13, rdi            ; r13 = List* list
    push r12
    mov r12, rsi            ; r12 = void* element
    push rbp
    mov ebp, edx            ; ebp = int size
    push rbx
    mov rbx, rcx            ; rbx = Compare_func_t compare_func

    ; Выравниваем стек (+48 байт, итого 64 с учетом push)
    sub rsp, 56
    and rsp, -16

    xor r15d, r15d          ; index = 0

.loop:
    ; Вычисляем адрес list->list_data[index] (index * 48)
    mov rax, r15
    imul rax, 48            ; rax = index * 48 (оптимально на современных CPU)
    add rax, [r13]          ; rax = &list->list_data[index]

    ; Копируем List_data (48 байт) с гарантированным выравниванием
    vmovdqu xmm0, [rax]
    vmovdqa [rsp], xmm0
    vmovdqu xmm1, [rax + 16]
    vmovdqa [rsp + 16], xmm1
    vmovdqu xmm2, [rax + 32]
    vmovdqa [rsp + 32], xmm2

    ; Вызываем compare_func(&tmp_list_el.element, element, size)
    lea rdi, [rsp + 16]     ; element находится по смещению 16 (prev+next=16)
    mov rsi, r12            ; элемент для сравнения
    mov edx, ebp            ; size
    call rbx

    ; Проверяем результат (Elements_equal == -1)
    cmp eax, -1
    je .found

    ; Переходим к следующему элементу (list_data.next)
    mov r15, [rsp + 8]      ; list_data.next
    test r15, r15
    jnz .loop

.not_found:
    mov rax, -1028          ; Not_found_value
    jmp .epilogue

.found:
    mov rax, r15            ; возвращаем индекс

.epilogue:
    add rsp, 56             ; восстанавливаем стек
    pop rbx
    pop rbp
    pop r12
    pop r13
    pop r14
    pop r15
    ret
