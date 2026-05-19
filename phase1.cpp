//phase1
Input.txt
$AMJ000100030001
GD10PD10H
$DTA
HELLO WORLD
$END0001

$AMJ000200030001
GD20PD20H
$DTA
SECOND JOB
$END0002

$AMJ000300030001
GD30PD30H
$DTA
THIRD JOB DATA
$END0003

$AMJ000400030001
GD40PD40H
$DTA
FOURTH JOB RUN
$END0004

$AMJ000500030001
GD50PD50H
$DTA
FIFTH JOB DONE
$END0005


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Memory and Registers
char M[100][4];
char IR[4];
char R[4];
int IC;
int SI;
char C;

// Job Control
int job_id, total_instr, total_output;

FILE *fin, *fout;

// Function Prototypes
void INIT();
void LOAD();
void STARTEXECUTION();
void EXECUTEUSERPROGRAM();
void MOS();
void READ();
void WRITE();
void TERMINATE();

void INIT() {
    memset(M, ' ', sizeof(M));
    memset(IR, ' ', sizeof(IR));
    memset(R, ' ', sizeof(R));
    IC = 0;
    SI = 0;
    C = 'F';
}

void LOAD() {
    char buffer[50];
    int m = 0;

    while (fgets(buffer, sizeof(buffer), fin)) {

        // $AMJ
        if (strncmp(buffer, "$AMJ", 4) == 0) {
            INIT();
            m = 0;

            char temp[5];

            strncpy(temp, buffer + 4, 4);
            temp[4] = '\0';
            job_id = atoi(temp);

            strncpy(temp, buffer + 8, 4);
            temp[4] = '\0';
            total_instr = atoi(temp);

            strncpy(temp, buffer + 12, 4);
            temp[4] = '\0';
            total_output = atoi(temp);
        }

        // $DTA
        else if (strncmp(buffer, "$DTA", 4) == 0) {
            STARTEXECUTION();
        }

        // $END
        else if (strncmp(buffer, "$END", 4) == 0) {
            char temp[5];
            strncpy(temp, buffer + 4, 4);
            temp[4] = '\0';

            int end_id = atoi(temp);

            if (end_id != job_id) {
                printf("ERROR: Job ID mismatch\n");
            }
        }

        // Program Cards
        else {
            int len = strlen(buffer);

            for (int i = 0; i < len; i += 4) {
                if (buffer[i] == '\n') break;

                for (int j = 0; j < 4; j++) {
                    M[m][j] = buffer[i + j];
                }
                m++;
            }
        }
    }
}

void STARTEXECUTION() {
    IC = 0;
    EXECUTEUSERPROGRAM();
}

void EXECUTEUSERPROGRAM() {
    while (1) {

        // Fetch
        for (int i = 0; i < 4; i++)
            IR[i] = M[IC][i];

        IC++;

        // Decode + Execute

        // LR
        if (IR[0]=='L' && IR[1]=='R') {
            int addr = (IR[2]-'0')*10 + (IR[3]-'0');
            memcpy(R, M[addr], 4);
        }

        // SR
        else if (IR[0]=='S' && IR[1]=='R') {
            int addr = (IR[2]-'0')*10 + (IR[3]-'0');
            memcpy(M[addr], R, 4);
        }

        // CR
        else if (IR[0]=='C' && IR[1]=='R') {
            int addr = (IR[2]-'0')*10 + (IR[3]-'0');

            if (memcmp(R, M[addr], 4) == 0)
                C = 'T';
            else
                C = 'F';
        }

        // BT
        else if (IR[0]=='B' && IR[1]=='T') {
            int addr = (IR[2]-'0')*10 + (IR[3]-'0');

            if (C == 'T')
                IC = addr;
        }

        // GD
        else if (IR[0]=='G' && IR[1]=='D') {
            SI = 1;
            MOS();
        }

        // PD
        else if (IR[0]=='P' && IR[1]=='D') {
            SI = 2;
            MOS();
        }

        // H
        else if (IR[0]=='H') {
            SI = 3;
            MOS();
            break;
        }
    }
}

void MOS() {
    switch (SI) {
        case 1: READ(); break;
        case 2: WRITE(); break;
        case 3: TERMINATE(); break;
    }
}

void READ() {
    char buffer[50];
    int addr = (IR[2]-'0')*10 + (IR[3]-'0');

    if (fgets(buffer, sizeof(buffer), fin)) {

        if (strncmp(buffer, "$END", 4) == 0)
            return;

        int k = 0;

        for (int i = addr; i < addr + 10; i++) {
            for (int j = 0; j < 4; j++) {
                if (buffer[k] == '\n') {
                    M[i][j] = ' ';
                } else {
                    M[i][j] = buffer[k++];
                }
            }
        }
    }
}

void WRITE() {
    int addr = (IR[2]-'0')*10 + (IR[3]-'0');

    for (int i = addr; i < addr + 10; i++) {
        for (int j = 0; j < 4; j++) {
            fputc(M[i][j], fout);
        }
    }
    fputc('\n', fout);
}

void TERMINATE() {
    fputc('\n', fout);
    fputc('\n', fout);
}

int main() {

    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    if (!fin || !fout) {
        printf("File error\n");
        return 1;
    }

    LOAD();

    fclose(fin);
    fclose(fout);

    printf("Execution Done. Check output.txt\n");
    return 0;
}
