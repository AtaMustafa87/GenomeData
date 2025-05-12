#include "ff.h" // Include the FatFs library for directory operations

void CreateNestedFoldersAndFile() {
   DXSPISDVOL disk(XPAR_PMODSD_0_AXI_LITE_SPI_BASEADDR,
         XPAR_PMODSD_0_AXI_LITE_SDCS_BASEADDR);
   FRESULT fr;
   static const char szDriveNbr[] = "0:";

   // Mount the disk
   DFATFS::fsmount(disk, szDriveNbr, 1);

   // Create the first folder
   fr = f_mkdir("0:/Folder1");
   if (fr == FR_OK) {
      xil_printf("Folder1 created successfully\r\n");

      // Create the nested folder inside Folder1
      fr = f_mkdir("0:/Folder1/NestedFolder");
      if (fr == FR_OK) {
         xil_printf("NestedFolder created successfully\r\n");

         // Create a file inside the nested folder
         DFILE file;
         fr = file.fsopen("0:/Folder1/NestedFolder/newfile.txt", FA_WRITE | FA_CREATE_ALWAYS);
         if (fr == FR_OK) {
            xil_printf("File created successfully in NestedFolder\r\n");
            u32 bytesWritten;
            fr = file.fswrite("Hello, World!", 13, &bytesWritten);
            if (fr == FR_OK) {
               xil_printf("Data written to file successfully\r\n");
            } else {
               xil_printf("Failed to write data to file\r\n");
            }
            file.fsclose();
         } else {
            xil_printf("Failed to create file in NestedFolder\r\n");
         }
      } else {
         xil_printf("Failed to create NestedFolder\r\n");
      }
   } else {
      xil_printf("Failed to create Folder1\r\n");
   }
}
void DemoRun() {
   DXSPISDVOL disk(XPAR_PMODSD_0_AXI_LITE_SPI_BASEADDR,
         XPAR_PMODSD_0_AXI_LITE_SDCS_BASEADDR);
   DFILE file;

   // The drive to mount the SD volume to.
   // Options are: "0:", "1:", "2:", "3:", "4:"
   static const char szDriveNbr[] = "0:";

   FRESULT fr;
   u32 bytesWritten = 0;
   u32 bytesRead, totalBytesRead;
   u8 buff[12], *buffptr;

   xil_printf("PmodSD Demo Launched\r\n");
   // Mount the disk
   DFATFS::fsmount(disk, szDriveNbr, 1);

   xil_printf("Disk mounted\r\n");

   fr = file.fsopen("newfile.txt", FA_WRITE | FA_CREATE_ALWAYS);
   if (fr == FR_OK) {
      xil_printf("Opened newfile.txt\r\n");
      fr = file.fswrite("It works!!!", 12, &bytesWritten);
      if (fr == FR_OK)
         xil_printf("Write successful\r\n");
      else
         xil_printf("Write failed\r\n");
      fr = file.fsclose();
      if (fr == FR_OK)
         xil_printf("File close successful\r\n");
      else
         xil_printf("File close failed\r\n");
   } else {
      xil_printf("Failed to open file to write to\r\n");
   }

   fr = file.fsopen("newfile.txt", FA_READ);
   if (fr == FR_OK) {
      buffptr = buff;
      totalBytesRead = 0;
      do {
         fr = file.fsread(buffptr, 1, &bytesRead);
         buffptr++;
         totalBytesRead += bytesRead;
      } while (totalBytesRead < 12 && fr == FR_OK);

      if (fr == FR_OK) {
         xil_printf("Read successful:");
         buff[totalBytesRead] = 0;
         xil_printf("'%s'\r\n", buff);
      } else {
         xil_printf("Read failed\r\n");
      }
   } else {
      xil_printf("Failed to open file to read from\r\n");
   }

   while (1);
}
