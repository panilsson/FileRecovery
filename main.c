
#include "cmninc.h"

UINT4 gu4BlockSize = 4096; /* Block Size */
int Block_size = 4096; // super block

int counter = 0;

UINT8 u8inodeBitmapOffset; /* offset in bytes to inode bitmap */
UINT4 freeInodeNumber; /* Free inode that will be written to */
UINT4 gu4FileDes;  /* File descriptor to the device */
struct ext4_super_block sb;
struct ext3_inode recoveredFile;
UINT4 gu4RootDataBlock;
UINT4 dirBlockNo;
long long Data_blocks_per_group = 32768; // super block
int gTotalNumberOfBlocks = 488025;

long long First_data_block_location = 1024 + (400 * 4096); //Can be found from group descriptor


// write disk name as ./digitalForensics.out /dev/<device_name>

// to read an inode do sudo ./digitalForensics /dev/sda1 1 <inode_num>
// to read a data block do sudo ./digitalForensics /dev/sda1 3 <block_num>
// to recover do sudo ./digitalForensics /dev/sda1 2
// by default, recovered files are written to node number 12
VOID main(INT4 argc, CHAR** argv){
    printf("%s\n", argv[2]);
    
    //struct ext4_inode Inode
    
    if(argv[2][0] == '1'){

        CHAR *pDev = argv[1];
        findInode(pDev, atoi(argv[3]));
    }
    else if(argv[2][0] == '3'){

        CHAR *pDev = argv[1];
        printf("opening %s\n", pDev);

        gu4FileDes = 0;
        int status = gu4FileDes = open(pDev, O_RDWR);
        if(status == -1){
            printf("Failed to open device\n");

            exit(-1);
        }
        else
            printf("Using device %s\n", pDev);
        
        printDataBlock(atoi(argv[3]));

    }
    else{
    CHAR *pDev = argv[1];
    printf("opening %s\n", pDev);

    gu4FileDes = 0;
    int status = gu4FileDes = open(pDev, O_RDWR);
    if(status == -1){
        printf("Failed to open device\n");

        exit(-1);
    }
    else
        printf("Using device %s\n", pDev);
    char *pBuffer;
    char Buffer[gu4BlockSize];
    memset(Buffer, 0, sizeof(Buffer));
    pBuffer = Buffer;
    
    // print block number
    findInode(pDev, 2); // finds the root directory data block
    //findInode(pDev,13);
    //findInode(pDev, 12);
    readAllDataBlocks(pDev);
   // printDataBlock(9216);
    findInode(pDev, 12);
        printf("%s\n", argv[1]);}
    
}

//given a block number this function returns the blockNum of the indirect address block
int findIndirectAddressBlock(int fd, int blockNum) {
    int i = 0, j = 0;
    unsigned char b[Block_size];
    unsigned char directBlock0[4];
    int loop_counter = 0;

    for (loop_counter = 0; loop_counter < gTotalNumberOfBlocks; loop_counter++) {
        //if ((gBlockBitmap[loop_counter] == FIRST_LEVEL_INDIRECT_BLOCK)||(gBlockBitmap[loop_counter] == ADDR_BLOCK))
        if (1) {
            int *buffer = (int *) malloc(gu4BlockSize);
            long offset = loop_counter * gu4BlockSize;
            lseek64(fd, offset, SEEK_SET);
            read(fd, (void *) buffer, gu4BlockSize);
            int i = 0;
            for (i = 0; i < gu4BlockSize / (sizeof(int)); i++) {
                if (buffer[i] == blockNum) {
                    printf("found indirect address block numbered %d\n", loop_counter);
                    //printDataBlock(loop_counter);
                    return loop_counter;
                }
            }
            free(buffer);
        }
    }
    printf("WAS UNABLE TO FIND THE FIRST_LEVEL_INDIRECT_BLOCK\n");
    return -1;
}

/*
This utility function is used to get the hexadecimal address of the block address
*/
char *decimalToHexStringInReverseOrder(int decimalNumber) {
    char *signs = (char *) malloc(sizeof(char) * 4);
    signs[0] = decimalNumber & 0xff;
    signs[1] = (decimalNumber >> 8) & 0xff;
    signs[2] = (decimalNumber >> 16) & 0xff;
    signs[3] = (decimalNumber >> 24) & 0xff;

    return signs;
}

/*
This utility function comapres the hex values of two character arrays upto a certain length (n)
*/
int compareHexValues(unsigned char string1[], unsigned char string2[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if ((int) string1[i] == (int) string2[i]) {
            if (i == n - 1) {
                return 1;
            }
        } else {
            return 0;
        }
    }
    return 0;
}


/*
 readSuperBlock() initializes the super block
 Originally made for ext4 super blocks but still works for ext3
 recovery.
 */
VOID readSuperBlock(CHAR* pDev){
    INT4 ret = 0;
    
    //i4RetVal = InodeUtilReadDataBlock(0, BOOT_SECTOR_OFFSET, &sb,
    //        sizeof (struct ext4_super_block));
    
    UINT8 offset;
    offset = ((UINT8) 4096 * 0) + 1024;
    
    lseek(gu4FileDes, offset, SEEK_SET);
    
    read(gu4FileDes, &sb, sizeof(struct ext4_super_block));

    //printf("%d\n", sb.s_inodes_count);
    
    
    gu4BlockSize = 1024 << sb.s_log_block_size;
    //gu4TotalInodes = sb.s_inodes_count;
    
}

/*
 writeInode Writes an inode to a specific inode number.
 */
VOID writeInode(CHAR* pDev, UINT4 u4InodeNo){
    // get inode offset to inode number
    
        //struct ext3_inode Inode;
        //memset(&Inode, 0, sizeof(Inode));
        UINT4 u4BlockNo;
        UINT2 u2Start;

        
        struct ext3_group_desc GroupDes;
        memset(&GroupDes, 0, sizeof(GroupDes));
        
        UINT4 u4GroupNo = 0; // since it is root directory
        UINT4 u4LocalInodeIndex;
        UINT8 u8GbdOffset = 0;

        // get inode of file N (let this be inode N)
        // find block group of inode N (let this be BGN)
        // goto block group BGN
        // goto inode table
        // find offset into inode table
        // typecast buffer into inode struct
        
        // goto root inode directory
        // typecast root inode to inode struct
        // get block number that holds list of files/directories in root directory
        // add new entry to this list (inode number from the file inode)
        //
        
        gu4FileDes = 0;
        int status = gu4FileDes = open(pDev, O_RDWR);
        if(status == -1){
            printf("Failed to open device\n");

            exit(-1);
        }
        else
            printf("Using device %s\n", pDev);
        
        // read in superblock
        readSuperBlock(pDev);
        
        // get inode offset
        u4GroupNo = (u4InodeNo - 1) / sb.s_inodes_per_group;
        
        u4LocalInodeIndex = (u4InodeNo - 1) % sb.s_inodes_per_group;
        
        // read in group des for BG
        u8GbdOffset = gu4BlockSize + u4GroupNo * sizeof(struct ext3_group_desc);
        //printf("inode index: %d\n", u4LocalInodeIndex);

        
        //i4RetVal = InodeUtilReadDataOffset(u8GbdOffset, &GroupDes, sizeof(struct //ext3_group_desc));
        
        lseek(gu4FileDes, u8GbdOffset, SEEK_SET);
        read(gu4FileDes, &GroupDes, sizeof(struct ext3_group_desc));
        
        int offsetInodeTable = ( GroupDes.bg_inode_table * gu4BlockSize) + (u4LocalInodeIndex * sb.s_inode_size);
    
        
        GET_BLOCK_OFFSET_FROM_BYTE_OFFSET(u4BlockNo, u2Start, offsetInodeTable);
        UINT8 u8Offset;
        u8Offset = ((UINT8) gu4BlockSize * u4BlockNo) + u2Start;
        
        // write inode to disk
        lseek64(gu4FileDes, u8Offset, SEEK_SET);
    //InodeUtilDumpInode(Inode);
        write(gu4FileDes, &recoveredFile, sizeof(struct ext3_inode));
    
        // link inode to root
    
        // create a directory entry for this file
        // append to root inode directory list
    // (inodedir.c) INT4 InodeDirAddChildEntry(struct ext3_dir_entry_2 *pNewChild, UINT4 u4BlockNo)

}

/*
 findInode Locates and prints an inode.
 */
VOID findInode(CHAR* pDev, UINT4 u4InodeNo){
        struct ext3_inode Inode;
        memset(&Inode, 0, sizeof(Inode));
        UINT4 u4BlockNo;
        UINT2 u2Start;

        
        struct ext3_group_desc GroupDes; // change this to ext4
        memset(&GroupDes, 0, sizeof(GroupDes));
        
        UINT4 u4GroupNo = 0; // since it is root directory
        UINT4 u4LocalInodeIndex;
        UINT8 u8GbdOffset = 0;

        // get inode of file N (let this be inode N)
        // find block group of inode N (let this be BGN)
        // goto block group BGN
        // goto inode table
        // find offset into inode table
        // typecast buffer into inode struct
        
        // goto root inode directory
        // typecast root inode to inode struct
        // get block number that holds list of files/directories in root directory
        // add new entry to this list (inode number from the file inode)
        //
        
        gu4FileDes = 0;
        int status = gu4FileDes = open(pDev, O_RDWR);
        if(status == -1){
            printf("Failed to open device\n");

            exit(-1);
        }
        else
            printf("Using device %s\n", pDev);
        
        // read in superblock
        readSuperBlock(pDev);
        
        // get inode offset
        u4GroupNo = (u4InodeNo - 1) / sb.s_inodes_per_group;
        
        u4LocalInodeIndex = (u4InodeNo - 1) % sb.s_inodes_per_group;
        
        // read in group des for BG
        u8GbdOffset = gu4BlockSize + u4GroupNo * sizeof(struct ext3_group_desc);
        //printf("inode index: %d\n", u4LocalInodeIndex);

        
        //i4RetVal = InodeUtilReadDataOffset(u8GbdOffset, &GroupDes, sizeof(struct //ext3_group_desc));
        
        lseek(gu4FileDes, u8GbdOffset, SEEK_SET);
        read(gu4FileDes, &GroupDes, sizeof(struct ext3_group_desc));
        
        int offsetInodeTable = ( GroupDes.bg_inode_table * gu4BlockSize) + (u4LocalInodeIndex * sb.s_inode_size);
    
        
        GET_BLOCK_OFFSET_FROM_BYTE_OFFSET(u4BlockNo, u2Start, offsetInodeTable);
        UINT8 u8Offset;
        u8Offset = ((UINT8) gu4BlockSize * u4BlockNo) + u2Start;
        
        lseek64(gu4FileDes, u8Offset, SEEK_SET);
        read(gu4FileDes, &Inode, sizeof(struct ext3_inode));
        
        //lseek(gu4FileDes, ( GroupDes.bg_inode_table_hi * gu4BlockSize) + (u4LocalInodeIndex * sb.s_inode_size), SEEK_CUR);
    //    read(gu4FileDes, &Inode, sizeof(struct ext4_inode));
        
        InodeUtilDumpInode(Inode);
        dirBlockNo = Inode.i_block[0];
        //printf("Directory block number: %d\n", dirBlockNo);
    if(u4InodeNo == 2){
        gu4RootDataBlock = dirBlockNo;
    }
        //appendInode(dirBlockNo);
        
        //InodeUtilDumpInode(&Inode);
        // first, find root inode offset
        // since it is the root inode, we know that the blockgroup # is 0
}

/*
 Broken function
 */
VOID linkToRoot(CHAR* pDev, UINT4 u4InodeNo){
        struct ext3_inode Inode;
        memset(&Inode, 0, sizeof(Inode));
        UINT4 u4BlockNo;
        UINT2 u2Start;

        
        struct ext3_group_desc GroupDes; // change this to ext4
        memset(&GroupDes, 0, sizeof(GroupDes));
        
        UINT4 u4GroupNo = 0; // since it is root directory
        UINT4 u4LocalInodeIndex;
        UINT8 u8GbdOffset = 0;

        // get inode of file N (let this be inode N)
        // find block group of inode N (let this be BGN)
        // goto block group BGN
        // goto inode table
        // find offset into inode table
        // typecast buffer into inode struct
        
        // goto root inode directory
        // typecast root inode to inode struct
        // get block number that holds list of files/directories in root directory
        // add new entry to this list (inode number from the file inode)
        //
        
        gu4FileDes = 0;
        int status = gu4FileDes = open(pDev, O_RDWR);
        if(status == -1){
            printf("Failed to open device\n");

            exit(-1);
        }
        else
            printf("Using device %s\n", pDev);
        
        // read in superblock
        readSuperBlock(pDev);
        
        // get inode offset
        u4GroupNo = (u4InodeNo - 1) / sb.s_inodes_per_group;
        
        u4LocalInodeIndex = (u4InodeNo - 1) % sb.s_inodes_per_group;
        
        // read in group des for BG
        u8GbdOffset = gu4BlockSize + u4GroupNo * sizeof(struct ext3_group_desc);
        //printf("inode index: %d\n", u4LocalInodeIndex);

        
        //i4RetVal = InodeUtilReadDataOffset(u8GbdOffset, &GroupDes, sizeof(struct //ext3_group_desc));
        
        lseek(gu4FileDes, u8GbdOffset, SEEK_SET);
        read(gu4FileDes, &GroupDes, sizeof(struct ext3_group_desc));
        
        int offsetInodeTable = ( GroupDes.bg_inode_table * gu4BlockSize) + (u4LocalInodeIndex * sb.s_inode_size);
    
        
        GET_BLOCK_OFFSET_FROM_BYTE_OFFSET(u4BlockNo, u2Start, offsetInodeTable);
        UINT8 u8Offset;
        u8Offset = ((UINT8) gu4BlockSize * u4BlockNo) + u2Start;
        
        lseek64(gu4FileDes, u8Offset, SEEK_SET);
        read(gu4FileDes, &Inode, sizeof(struct ext3_inode));
        
        //lseek(gu4FileDes, ( GroupDes.bg_inode_table_hi * gu4BlockSize) + (u4LocalInodeIndex * sb.s_inode_size), SEEK_CUR);
    //    read(gu4FileDes, &Inode, sizeof(struct ext4_inode));
        
        InodeUtilDumpInode(Inode);
        dirBlockNo = Inode.i_block[0];
        printf("Directory block number: %d\n", dirBlockNo);
    
        // we have the block number, read it into a buffer
        // once read into a buffer, append the inode entry to it and
        // write it to the disk
    
        //appendInode(dirBlockNo);
        
        //InodeUtilDumpInode(&Inode);
        // first, find root inode offset
        // s
}

/*
 Updated version of appendInode, works for ext3 instead of ext4.
 */
VOID appendInode3(UINT4 blockNo, UINT4 inodeNum){
    struct ext3_dir_entry_2 DirEntry;
    struct ext3_dir_entry_2 pNewChild;
    struct ext3_dir_entry_2 tmp;
    
    CHAR *pBuffer;
    UINT4 u4Index;
    UINT4 u4NextPos;
    INT4 i4BytesReadSoFar = 0;
    UINT2 u2Len= 0;
    CHAR Buffer[gu4BlockSize];
    INT4 i4RetVal;
    UINT4 store = 0;
    
    memset(&DirEntry, 0, sizeof(DirEntry));
    memset(&pNewChild, 0, sizeof(pNewChild));
    memset(&tmp, 0, sizeof(tmp));

    memset(Buffer, 0, sizeof(Buffer));
    pBuffer = Buffer;
    u4Index = 0;
    u4NextPos = 0;
    i4RetVal = 0;
    
    //pBuffer = readtDataBlock(blockNo);
    
    UINT4 totalOffsetq = blockNo * gu4BlockSize;
    //printf("HERE3232 %d\n", totalOffsetq);

    lseek(gu4FileDes, totalOffsetq, SEEK_SET);
    read(gu4FileDes, pBuffer, gu4BlockSize);
    //printf("HERE3232\n");
    printf("HERE1\n");

    int step = 0;
    for (u4Index = 0; u4Index < gu4BlockSize; u4Index++) {
        memset(&DirEntry, 0, sizeof(DirEntry));
        CHAR *pPos = NULL;

        /* Parse a directory record entry in the data block */
        //i4RetVal = InodeDirReadRecord(pBuffer, u4NextPos, &DirEntry);
        
        pPos = pBuffer + u4NextPos;
        memcpy(&DirEntry, pPos, sizeof(UINT8));
        

        
        strncpy(DirEntry.name, pPos + DIR_ENTRY_NAME_OFFSET,
               DirEntry.name_len);
        
        //printDirectoryEntry(&DirEntry);
        
        /* Print the entry */
        //InodeUtilDumpDirEntry(&DirEntry, FILE_TYPE_FILES, u4Level);

        /* Move the offset to the start of next record */
        u4NextPos += DirEntry.rec_len;

        /* If end of records, break the loop */
        if (u4NextPos == gu4BlockSize) {

            break;
        }
        else
        {

            i4BytesReadSoFar = i4BytesReadSoFar + DirEntry.rec_len;

        }
    }
    //printDirectoryEntry(&pNewChild);
    modEntry(&pNewChild, inodeNum);
    printf("Entry to be added:\n\n");

    printDirectoryEntry(&pNewChild);
    DirEntry.rec_len = DIR_REC_LEN(DirEntry);
    memcpy(pBuffer + i4BytesReadSoFar, &DirEntry, DirEntry.rec_len);
    u2Len = DirEntry.rec_len;

    /* adding the new directory entry to the buffer*/
    memcpy(pBuffer + i4BytesReadSoFar + u2Len, &pNewChild, pNewChild.rec_len);

    // write this to the device...
    
    UINT8 offset2 = gu4BlockSize * dirBlockNo;

    int seekStatus = lseek64(gu4FileDes, offset2, SEEK_SET);
    int writeStatus = write(gu4FileDes, pBuffer, gu4BlockSize);
    if(seekStatus != 0){
        printf("SEEK SUCCESS %d\n", writeStatus);

    }
    else{
        printf("SEEK FAIL\n", writeStatus);

    }
    if(writeStatus != 0){
        printf("INODE LINK SUCCESS\n", writeStatus);

    }
    else{
        printf("INODE LINK FAIL\n", writeStatus);

    }
}

/*
 linkInode() is used when appending directory entries to ext4
 directories.
 */
VOID linkInode(CHAR* pDev, UINT4 u4InodeNo){
    struct ext4_inode Inode;
    memset(&Inode, 0, sizeof(Inode));
    UINT4 u4BlockNo;
    UINT2 u2Start;

    
    struct ext4_group_desc GroupDes; // change this to ext4
    memset(&GroupDes, 0, sizeof(GroupDes));
    
    UINT4 u4GroupNo = 0; // since it is root directory
    UINT4 u4LocalInodeIndex;
    UINT8 u8GbdOffset = 0;

    // get inode of file N (let this be inode N)
    // find block group of inode N (let this be BGN)
    // goto block group BGN
    // goto inode table
    // find offset into inode table
    // typecast buffer into inode struct
    
    // goto root inode directory
    // typecast root inode to inode struct
    // get block number that holds list of files/directories in root directory
    // add new entry to this list (inode number from the file inode)
    //
    
    gu4FileDes = 0;
    int status = gu4FileDes = open(pDev, O_RDWR);
    if(status == -1){
        printf("Failed to open device\n");

        exit(-1);
    }
    else
        printf("Using device %s\n", pDev);
    
    // read in superblock
    readSuperBlock(pDev);
    
    // get inode offset
    u4GroupNo = (u4InodeNo - 1) / sb.s_inodes_per_group;
    
    u4LocalInodeIndex = (u4InodeNo - 1) % sb.s_inodes_per_group;
    
    // read in group des for BG
    u8GbdOffset = gu4BlockSize + u4GroupNo * sizeof(struct ext4_group_desc);
    //printf("inode index: %d\n", u4LocalInodeIndex);

    
    //i4RetVal = InodeUtilReadDataOffset(u8GbdOffset, &GroupDes, sizeof(struct //ext3_group_desc));
    
    lseek(gu4FileDes, u8GbdOffset, SEEK_SET);
    read(gu4FileDes, &GroupDes, sizeof(struct ext4_group_desc));
    
    int offsetInodeTable = ( GroupDes.bg_inode_table_lo * gu4BlockSize) + (u4LocalInodeIndex * sb.s_inode_size);
    
    GET_BLOCK_OFFSET_FROM_BYTE_OFFSET(u4BlockNo, u2Start, offsetInodeTable);
    UINT8 u8Offset;
    u8Offset = ((UINT8) gu4BlockSize * u4BlockNo + 20) + u2Start;
    
    lseek64(gu4FileDes, u8Offset, SEEK_SET);
    read(gu4FileDes, &Inode, sizeof(struct ext4_inode));
    
    //lseek(gu4FileDes, ( GroupDes.bg_inode_table_hi * gu4BlockSize) + (u4LocalInodeIndex * sb.s_inode_size), SEEK_CUR);
//    read(gu4FileDes, &Inode, sizeof(struct ext4_inode));
    
    dirBlockNo = Inode.i_block[0];
    printf("Directory block number: %d\n", dirBlockNo);
    appendInode(dirBlockNo);
    
    //InodeUtilDumpInode(&Inode);
    // first, find root inode offset
    // since it is the root inode, we know that the blockgroup # is 0
    
}


// Find first block of inode
// Set up new child directory (should be file inode)...
// Find last entry, add newchild to end of first block
// since we're linking to root, level should always be 1
VOID appendInode(UINT4 blockNo){
    struct ext4_dir_entry_2 DirEntry;
    struct ext4_dir_entry_2 pNewChild;
    struct ext4_dir_entry_2 tmp;

    //printDataBlock(dirBlockNo);

    //printf("size %d", sizeof(struct ext4_dir_entry_2));

    /*
    pNewChild->inode = 12;
    pNewChild->rec_len = 12;
    pNewChild->name_len = 5;
    pNewChild->file_type = 2;
    //pNewChild->name[EXT4_NAME_LEN] = "docfi";*/
    
    //printf("HERE\n");

    //printDirectoryEntry(pNewChild);
    
    CHAR *pBuffer;
    UINT4 u4Index;
    UINT4 u4NextPos;
    INT4 i4BytesReadSoFar = 0;
    UINT2 u2Len= 0;
    CHAR Buffer[gu4BlockSize];
    INT4 i4RetVal;
    UINT4 store = 0;
    
    memset(&DirEntry, 0, sizeof(DirEntry));
    memset(&pNewChild, 0, sizeof(pNewChild));
    memset(&tmp, 0, sizeof(tmp));

    memset(Buffer, 0, sizeof(Buffer));
    pBuffer = Buffer;
    u4Index = 0;
    u4NextPos = 0;
    i4RetVal = 0;
    
    //pBuffer = readtDataBlock(blockNo);
    
    UINT4 totalOffsetq = blockNo * gu4BlockSize;
    //printf("HERE3232 %d\n", totalOffsetq);

    lseek(gu4FileDes, totalOffsetq, SEEK_SET);
    read(gu4FileDes, pBuffer, gu4BlockSize);
    //printf("HERE3232\n");
    
    int step = 0;
    for (u4Index = 0; u4Index < gu4BlockSize; u4Index++) {
        memset(&DirEntry, 0, sizeof(DirEntry));
        CHAR *pPos = NULL;

        /* Parse a directory record entry in the data block */
        //i4RetVal = InodeDirReadRecord(pBuffer, u4NextPos, &DirEntry);
        
        pPos = pBuffer + u4NextPos;
        memcpy(&DirEntry, pPos, sizeof(UINT8));
        

        
        strncpy(DirEntry.name, pPos + DIR_ENTRY_NAME_OFFSET,
               DirEntry.name_len);
        
        printDirectoryEntry(&DirEntry);
        
        if(step == 1){
            addExtEntry(&tmp);

            memcpy(&tmp, &DirEntry, sizeof(DirEntry));
            
            //DirEntry.rec_len = DIR_REC_LEN(DirEntry);
            //memcpy(pBuffer + i4BytesReadSoFar, &DirEntry, DirEntry.rec_len);
            u2Len = DirEntry.rec_len;
            memcpy(pBuffer + i4BytesReadSoFar + u2Len, &DirEntry, DirEntry.rec_len);
            step++;
        }
        
        if(DirEntry.inode == 12){
            memcpy(&pNewChild, &DirEntry, sizeof(DirEntry));
            modEntry(&pNewChild);

        }
        if(DirEntry.file_type == 222 && step == 0){
            store = u4Index;
            memcpy(&pNewChild, &DirEntry, sizeof(DirEntry));

            modEntry(&DirEntry);
            //DirEntry.rec_len = DIR_REC_LEN(DirEntry);
            //memcpy(pBuffer + i4BytesReadSoFar, &DirEntry, DirEntry.rec_len);
            u2Len = DirEntry.rec_len;
            memcpy(pBuffer + i4BytesReadSoFar + u2Len, &DirEntry, DirEntry.rec_len);
            
            step++;
        }
        
        /* Print the entry */
        //InodeUtilDumpDirEntry(&DirEntry, FILE_TYPE_FILES, u4Level);

        /* Move the offset to the start of next record */
        u4NextPos += DirEntry.rec_len;

        /* If end of records, break the loop */
        if (u4NextPos == gu4BlockSize) {
            break;
        }
        else
        {
            i4BytesReadSoFar = i4BytesReadSoFar + DirEntry.rec_len;
        }
    }

    modEntry(&pNewChild);
    printf("Entry to be added:\n\n");

    printDirectoryEntry(&pNewChild);
    DirEntry.rec_len = DIR_REC_LEN(DirEntry);
    memcpy(pBuffer + i4BytesReadSoFar, &DirEntry, DirEntry.rec_len);
    u2Len = DirEntry.rec_len;

    /* adding the new directory entry to the buffer*/
    memcpy(pBuffer + i4BytesReadSoFar + u2Len, &pNewChild, pNewChild.rec_len);
    
    UINT8 offset2 = gu4BlockSize * dirBlockNo;

    int seekStatus = lseek64(gu4FileDes, offset2, SEEK_SET);
    int writeStatus = write(gu4FileDes, pBuffer, gu4BlockSize);

    if(seekStatus != 0){
        printf("SEEK SUCCESS %d\n", writeStatus);

    }
    else{
        printf("SEEK FAIL\n", writeStatus);

    }
    if(writeStatus != 0){
        printf("INODE LINK SUCCESS\n", writeStatus);

    }
    else{
        printf("INODE LINK FAIL\n", writeStatus);

    }
    
    // This loop verifies that the entry was appended.
    // Issue: directory block is corrupted
    
    u4NextPos = 0;
    u4Index = 0;
    i4BytesReadSoFar = 0;
    u2Len = 0;
    //lseek(pBuffer, 0, SEEK_SET);

    for (u4Index = 0; u4Index < 20; u4Index++) {
        memset(&DirEntry, 0, sizeof(DirEntry));
        CHAR *pPos = NULL;

        
        pPos = pBuffer + u4NextPos;
        memcpy(&DirEntry, pPos, sizeof(UINT8));
        

        
        strncpy(DirEntry.name, pPos + DIR_ENTRY_NAME_OFFSET,
               DirEntry.name_len);
        
        printDirectoryEntry(&DirEntry);
        
        if(u4Index == store){
            memcpy(&pNewChild, &DirEntry, sizeof(DirEntry));

            addExtEntry(&DirEntry);
            //DirEntry.rec_len = DIR_REC_LEN(DirEntry);
            //memcpy(pBuffer + i4BytesReadSoFar, &DirEntry, DirEntry.rec_len);
            u2Len = DirEntry.rec_len;
            memcpy(pBuffer + i4BytesReadSoFar + u2Len, &DirEntry, DirEntry.rec_len);        }


        u4NextPos += DirEntry.rec_len;

        if (u4NextPos == gu4BlockSize) {
            break;
        }
        else
        {
            i4BytesReadSoFar = i4BytesReadSoFar + DirEntry.rec_len;
        }
    }
}

/*
 readAllDataBlocks() is the primary function used in recovering doc files.
 Scans the drive to find all blocks that start with the doc file signature
 and then reconstructs the inode and writes it to the disk.
 */
VOID readAllDataBlocks(CHAR* pDev){
    gu4FileDes = 0;
    int status = gu4FileDes = open(pDev, O_RDWR);
    if(status == -1){
        printf("Failed to open device\n");

        exit(-1);
    }
    else
        printf("Using device %s\n", pDev);
    char *pBuffer;
    char Buffer[gu4BlockSize];
    memset(Buffer, 0, sizeof(Buffer));
    pBuffer = Buffer;

    // given: partition size, num of block groups,
    // for bg in block groups
    //      for block in bg:
    //          read into buffer
    //          compare first 8 bytes with file signature
    //          if same
    //              print offset
    //              end program
    //          if false
    //              continue
    UINT4 blkgroupSize = gu4BlockSize * 8 * gu4BlockSize;

    for(UINT4 blkgroup = 0; blkgroup < 15; blkgroup++){
        UINT4 blkgroupOffset = (blkgroup * blkgroupSize);

        for(UINT4 blkno = 0; blkno < 134217728; blkno += gu4BlockSize){

            UINT4 totalOffset = blkgroupOffset + blkno;
            if(blkno == 0)
                printf("Reading blockgroup: %d\n",blkgroup);

            lseek(gu4FileDes, totalOffset, SEEK_SET);
            read(gu4FileDes, pBuffer, gu4BlockSize);
            //if(blkgroup == 0 && blkno >= 0 && blkno <= (gu4BlockSize * 2)){

            if(checkIfDoc(pBuffer) == 1 && counter == 0){
                printf("FOUND DOC FILE BEGINNING AT BLKNO: %d\n", (blkno/gu4BlockSize));
                counter = 1;
                printf("total offset: %d\n", totalOffset);
                printf("HERE\n");

                for(int j = 0; j < 12; j++){
                    //printf("HERE\n");

                    recoveredFile.i_block[j] = blkno/gu4BlockSize + j;
                    //printf("HERE2\n");

                    //lseek(gu4FileDes, totalOffset + (j * gu4BlockSize), SEEK_SET);
                    //read(gu4FileDes, pBuffer, gu4BlockSize);
                }
                //printf("HERE3\n");

                //InodeUtilDumpDataBlockRaw(pBuffer);
                
                // THIS CONSTRUCTS THE INODE
                // then... write this inode to the disk and link to root using the function
                // afterwards, the file should be 'recovered'
                int indirect_addr =findIndirectAddressBlock(gu4FileDes, (blkno/gu4BlockSize) + 12);
                recoveredFile.i_block[12] = indirect_addr;
                
                recoveredFile.i_mode = 33188;
                recoveredFile.i_uid = 1000;
                recoveredFile.i_size = 1000;
                recoveredFile.i_ctime = 312312;
                recoveredFile.i_atime = 312312;
                recoveredFile.i_mtime = 312312;
                recoveredFile.i_dtime = 312312;
                recoveredFile.i_flags = 312312;
                recoveredFile.i_generation = 312312;
                recoveredFile.i_gid = 1000;
                recoveredFile.i_blocks = 168;
                recoveredFile.i_links_count = 1;
                recoveredFile.i_extra_isize = 32;
                //recoveredFile.osd2.linux2 = 12323232;
                
                writeInode(pDev, 12);
                appendInode3(gu4RootDataBlock, 12);
                
                //InodeUtilDumpInode(recoveredFile);

            }
          // }
            


            pBuffer = Buffer;
            
        }
    }
}

/*! *************************************************************************
 * Function Name : InodeUtilDumpDataBlockRaw
 *
 * Author        : Naveen Raj Selvaraj
 *
 * Description   : This Function is used to print the content of a block in HEX
 *
 * Input         : pBuffer  -   buffer containing the data block contents
 *
 * Output        : None
 *
 * Returns       : None
 *
 **************************************************************************/
VOID InodeUtilDumpDataBlockRaw(CHAR *pBuffer) {
    UINT4 u4Index1 = 0;
    UINT4 u4Index2 = 0;
    for (u4Index1 = 0; u4Index1 < 512; u4Index1++) {
        fprintf(stdout, " %02X", (unsigned char) pBuffer[u4Index1]);
        u4Index2 += 1;
        if (u4Index2 == 16) {
            fprintf(stdout, "\n");
            u4Index2 = 0;
        }
    }
     fprintf(stdout, "\n\n END OF BLOCK \n\n");
}

/*
 Function used to create directory entries
 */
VOID modEntry(struct ext3_dir_entry_2 *entry, UINT4 inodeNum){
    entry->inode = inodeNum;
    entry->rec_len = 12;
    entry->name_len = 4;
    entry->file_type = 1;
    strcpy(entry->name, "test");
}

/*
 Old function used in create ext entries for ext4
 */
VOID addExtEntry(struct ext3_dir_entry_2 *entry){
    entry->inode = 0;
    entry->rec_len = 12;
    entry->name_len = 0;
    entry->file_type = 222;
    strcpy(entry->name, "");
}


// this method overwrites the block at blockNo (int)
int writeDataBlock(UINT4 blockNo, VOID *pBuffer){
    UINT4 offset = gu4BlockSize * blockNo;
    
    lseek(gu4FileDes, offset, SEEK_SET);
    int writeStatus = write(gu4FileDes, pBuffer, gu4BlockSize);
    return writeStatus;
}

/*
 printDataBlock() prints the data block at the specified block number.
 */
VOID printDataBlock(UINT4 blockNo){
    char *pBuffer;
    char Buffer[gu4BlockSize];
    memset(Buffer, 0, sizeof(Buffer));
    pBuffer = Buffer;
    
    UINT4 totalOffset = blockNo * gu4BlockSize;
    
    lseek(gu4FileDes, totalOffset, SEEK_SET);
    read(gu4FileDes, pBuffer, 512);
    
    InodeUtilDumpDataBlockRaw(pBuffer);
    
}

/*
 printDirectoryEntry prints a directory entry for ext4.
 */
VOID printDirectoryEntry(struct ext4_dir_entry_2 *dir){
    printf(" Inode = %hu\n", dir->inode);
    printf(" Record Length = %hu\n", dir->rec_len);
    printf(" Name length = %hu\n", dir->name_len);
    printf(" File type = %hu\n", dir->file_type);
    printf(" Name = %s\n\n", dir->name);
    
}

CHAR * readtDataBlock(UINT4 blockNo){
    CHAR *pBuffer;
    CHAR Buffer[gu4BlockSize];
    memset(Buffer, 0, sizeof(Buffer));
    pBuffer = Buffer;
    
    UINT4 totalOffset = blockNo * gu4BlockSize;
    
    lseek(gu4FileDes, totalOffset, SEEK_SET);
    read(gu4FileDes, pBuffer, gu4BlockSize);
    return pBuffer;
}

/*
 Checks if a file is a doc file or not by checking the first 8 bytes of the
 data block.
 */
int checkIfDoc(CHAR *pBuffer) {
    UINT4 u4Index1 = 0;
    UINT4 u4Index2 = 0;
    const unsigned char header[] = { 0xd0, 0xcf, 0x11, 0xe0, 0xa1, 0xb1, 0x1a, 0xe1};


    int result;

    if(memcmp(pBuffer, header, sizeof header) == 0)
    {
        return 1;
    }
    else{
        return 0;
    }

}

/*
 Prints an inode.
 */
VOID InodeUtilDumpInode(struct ext3_inode pInode) {
    char *pBuffer;
    char Buffer[512];
    memset(Buffer, 0, sizeof(Buffer));
    pBuffer = Buffer;

    UINT4 u4Index = 0;
    printf(" i_mode = %hu\n", pInode.i_mode);
    printf(" i_uid = %hu\n", pInode.i_uid);
    printf(" i_size = %hu\n", pInode.i_uid);
    printf(" l_i_frag = %hu\n", pInode.osd2);
    printf(" i_atime = %u\n", pInode.i_atime);
    printf(" i_ctime = %u\n", pInode.i_ctime);
    printf(" i_mtime = %u\n", pInode.i_mtime);
    printf(" i_dtime = %u\n", pInode.i_dtime);
    printf(" i_gid = %hu\n", pInode.i_gid);
    printf(" i_blocks = %hu\n", pInode.i_blocks);

    printf(" i_links_count = %hu\n", pInode.i_links_count);

    for (u4Index = 0; u4Index < EXT4_N_BLOCKS; u4Index++) {
        printf("    i_block[%d] = %u\n", u4Index, pInode.i_block[u4Index]);

        
        if(u4Index == 0){
            UINT4 totalOffset = pInode.i_block[u4Index] * gu4BlockSize;
            //printf("total offset: %d\n", totalOffset);
            lseek(gu4FileDes, totalOffset, SEEK_SET);
            read(gu4FileDes, pBuffer, 512);
            
            //InodeUtilDumpDataBlockRaw(pBuffer);
        }
    }
    printf(" i_flags = %u\n", pInode.i_flags);
    //printf(" acl = %u\n", pInode.linux2);

    printf(" i_generation = %u\n", pInode.i_generation);
    printf(" i_extra_isize = %hu\n", pInode.i_extra_isize);
}
