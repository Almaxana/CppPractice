// works in Visual Studio; input data is in input.txt has to be one string (example: 9 8 5 4 3 2 1)

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
int main()
{
    FILE* inputFile;
    fopen_s(&inputFile, "input.txt", "r");
    if (inputFile == NULL) {
        std::cout << "Cannot open the file!";
        return 0;
    }

    int n = 0;
    int arr[1000];
    while (fscanf_s(inputFile, "%d", &arr[n]) != EOF && n < 1000) {
        n++;
    }
    fclose(inputFile);

    
    _asm
    {
        mov EAX, 4 // size
        mov EBX, 0 // i
        mov ECX, n
        mul ECX //n * 4 -> EAX
        mov ECX, 4 // j
        lea ESI, arr //adress of arr -> ESI

        for_i :
            cmp EBX, EAX // if i >= size
                jge end // finish the sort

            mov ECX, 4
            for_j :
                cmp ECX, EAX // if j >= size
                    jge end_i


                mov EDX, dword ptr[ESI + ECX]
                cmp dword ptr[ESI + ECX - 4], EDX //arr[j - 1] <= arr[j]
                    jle end_j

                xchg EDX, dword ptr[ESI + ECX - 4]
                xchg EDX, dword ptr[ESI + ECX]

                end_j:
                    add ECX, 4
                    jmp for_j

            end_i :
                add EBX, 4
                jmp for_i
     end :
    }
    
    FILE* outputFile;
    fopen_s(&outputFile, "output.txt", "w");
    if (outputFile == NULL) {
        std::cout << "Cannot open the file";
        return 0;
    }
    for (int i = 0; i < n; i++) {
        fprintf(outputFile, "%d\n", arr[i]);
        std::cout << arr[i] << " ";
    }
    fclose(outputFile);
}
