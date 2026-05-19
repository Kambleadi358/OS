#include <stdio.h>

int main()
{
    int logicalAddress;
    int pageSize = 2;   // given
    int pageNo, offset;
    int frameNo;
    int pageTable[4];   // since 8 bytes / 2 = 4 pages

    printf("Paging Address Translation\n");

    // Input Page Table
    printf("Enter Page Table (4 entries for P0 P1 P2 P3):\n");
    for(int i = 0; i < 4; i++)
    {
        printf("Frame number for Page P%d: ", i);
        scanf("%d", &pageTable[i]);
    }

    // Input Logical Address (0-7)
    printf("\nEnter Logical Address (0 to 7): ");
    scanf("%d", &logicalAddress);

    if(logicalAddress < 0 || logicalAddress > 7)
    {
        printf("Invalid Logical Address\n");
        return 0;
    }

    // Step 1: Page Number
    pageNo = logicalAddress / pageSize;

    // Step 2: Offset
    offset = logicalAddress % pageSize;

    // Step 3: Frame Number
    frameNo = pageTable[pageNo];

    // Step 4: Physical Address
    int physicalAddress = frameNo * pageSize + offset;

    // OUTPUT (STEP BY STEP)
    printf("\n--- Calculation Steps ---\n");
    printf("Page No = LA / Page Size = %d / %d = %d\n", logicalAddress, pageSize, pageNo);
    printf("Offset  = LA %% Page Size = %d %% %d = %d\n", logicalAddress, pageSize, offset);
    printf("Frame No = PageTable[%d] = %d\n", pageNo, frameNo);
    printf("Physical Address = Frame * Page Size + Offset\n");
    printf("= %d * %d + %d = %d\n", frameNo, pageSize, offset, physicalAddress);

    // Binary Output
    printf("\nLogical Address (3-bit)  = %03d\n", logicalAddress);
    printf("Physical Address (4-bit) = %04d\n", physicalAddress);

    return 0;
}
