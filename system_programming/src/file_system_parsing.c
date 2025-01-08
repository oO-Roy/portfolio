/* 	Developer: Roy Steiner.
	File: File system parsing (C).
	Reviewer: Yehonatan Peleg.  
	Date: 17/11/2024.
	Version 1.0
	Status:  APPROVED. */

#define _XOPEN_SOURCE 500 /*pread*/

#include <stdio.h> /*printf, fprintf*/
#include <string.h> /*memcpy, strcmp, strcpy, strtok*/
#include <linux/ext2_fs.h> /*API*/
#include <fcntl.h> /*O_RDONLY*/
#include <unistd.h> /*pread, size_t, ssize_t*/
#include <sys/stat.h> /*S_ISDIR*/
#include <libgen.h> /*basename, dirname*/
#include <assert.h>

#define MIN(a,b) ((a > b) * b) + ((b > a) * a)
#define BLOCK_OFFSET(block) (block * (g_block_size))

#define NAME_SIZE 80
#define BLOCK_SIZE 4096
#define ERROR -1
#define BASE_OFFSET 0x400 /* beginning of the superblock (first group). 
                                                                0x400 = 1024 */

typedef struct ext2_super_block super_t;
typedef struct ext2_group_desc group_t;
typedef struct ext2_inode inode_t;
typedef struct ext2_dir_entry_2 entry_t;

super_t g_super;
group_t g_group;
inode_t g_inode;

static unsigned int g_block_size = 0;

int g_fd = 0;
char g_dev_name[NAME_SIZE];

static void PrintSuperblock(void)
{
    printf("Reading super-block from device: %s\n"
	       "Inodes count            : %u\n"
	       "Blocks count            : %u\n"
	       "Reserved blocks count   : %u\n"
	       "Free blocks count       : %u\n"
	       "Free inodes count       : %u\n"
	       "First data block        : %u\n"
	       "Block size              : %u\n"
	       "Blocks per group        : %u\n"
	       "Inodes per group        : %u\n"
	       "Creator OS              : %u\n"
	       "First non-reserved inode: %u\n"
	       "Size of inode structure : %hu\n"
	       ,
           g_dev_name,
	       g_super.s_inodes_count,
	       g_super.s_blocks_count,
	       g_super.s_r_blocks_count,
	       g_super.s_free_blocks_count,
	       g_super.s_free_inodes_count,
	       g_super.s_first_data_block,
	       g_block_size,
	       g_super.s_blocks_per_group,
	       g_super.s_inodes_per_group,
	       g_super.s_creator_os,
	       g_super.s_first_ino,
	       g_super.s_inode_size);
}

static void PrintGroupDescriptor(void)
{
    printf("\nReading first group-descriptor from device: %s\n"
	       "Blocks bitmap block: %u\n"
	       "Inodes bitmap block: %u\n"
	       "Inodes table block : %u\n"
	       "Free blocks count  : %u\n"
	       "Free inodes count  : %u\n"
	       "Directories count  : %u\n"
	       ,
           g_dev_name,
	       g_group.bg_block_bitmap,
	       g_group.bg_inode_bitmap,
	       g_group.bg_inode_table,
	       g_group.bg_free_blocks_count,
	       g_group.bg_free_inodes_count,
	       g_group.bg_used_dirs_count);
}

static void PrintInode(void)
{
	size_t i = 0;
	
	printf("\nReading root inode\n"
	       "File mode: %hu\n"
	       "Owner UID: %hu\n"
	       "Size     : %u bytes\n"
	       "Blocks   : %u\n"
	       ,
	       g_inode.i_mode,
	       g_inode.i_uid,
	       g_inode.i_size,
	       g_inode.i_blocks);
	
	for(i = 0; i < EXT2_N_BLOCKS; ++i)
	if (i < EXT2_NDIR_BLOCKS)
		printf("Block %2lu : %u\n", i, g_inode.i_block[i]);
	else if (i == EXT2_IND_BLOCK)
		printf("Single   : %u\n", g_inode.i_block[i]);
	else if (i == EXT2_DIND_BLOCK)
		printf("Double   : %u\n", g_inode.i_block[i]);
	else if (i == EXT2_TIND_BLOCK)
		printf("Triple   : %u\n", g_inode.i_block[i]);
}

static int ReadSuperblock(void)
{
	if (ERROR == pread(g_fd, &g_super, sizeof(super_t), BASE_OFFSET)
										|| g_super.s_magic != EXT2_SUPER_MAGIC) 
    {
		fprintf(stderr, "Not a EXT2 File-system\n");
		return ERROR;
	}

    g_block_size = EXT2_MIN_BLOCK_SIZE << g_super.s_log_block_size;
	PrintSuperblock();

	return 0;
}

static int ReadGroupDescriptors(void)
{
	if (ERROR == pread(g_fd, &g_group, sizeof(group_t), g_block_size)) 
    {
		return ERROR;
	}

	PrintGroupDescriptor();

	return 0;
}

static size_t InodeTableOffset(unsigned int inode_num)
{
    group_t local_group;
    size_t grp_index = inode_num / g_super.s_inodes_per_group;

    pread(g_fd, &local_group, sizeof(group_t), g_block_size 
            					+ (grp_index * sizeof(group_t)));

    return BLOCK_OFFSET(local_group.bg_inode_table);
}

static int ReadInode(unsigned int inode_idx)
{
	size_t offset = InodeTableOffset(inode_idx) 
    + g_super.s_inode_size * ((inode_idx % g_super.s_inodes_per_group) - 1);

	if (ERROR ==  pread(g_fd, &g_inode, (g_super.s_inode_size / 2), offset))
    {
		return ERROR;
	}

	PrintInode();

	return 0;
}

static int ReadEntry(const char* file_name, unsigned int* traget_inode)
{
	size_t entry_size = 0;
	unsigned char block[BLOCK_SIZE];
	entry_t* entry;
	
	if (BLOCK_SIZE != pread(g_fd, block, BLOCK_SIZE
										, (g_inode.i_block[0] * g_block_size)))
	{
		return ERROR;
	}

	entry = (entry_t*) block;

	while(entry_size < g_inode.i_size) 
	{
        char fname[EXT2_NAME_LEN+1];
        memcpy(fname, entry->name, entry->name_len);
        fname[entry->name_len] = 0;
        printf("%10u %s\n", entry->inode, fname);
        entry = (entry_t*)(((char*)entry) + entry->rec_len);
        entry_size += entry->rec_len;
		
		if (!strcmp(file_name, entry->name))
		{
			printf("%10u %s\n", entry->inode, entry->name);
			*traget_inode = entry->inode;
			break;
		}
		
		else if (0 == entry->name_len)
		{
			return ERROR;
		}
	}

	return 0;
}

static int ReadEdgeFolder(char* folder_name, unsigned int* traget_inode)
{
	folder_name = strtok(folder_name, "/");

	while (folder_name)
	{
		if (0 != ReadEntry(folder_name, traget_inode))
		{
			return ERROR;
		}

		if (0 != ReadInode(*traget_inode))
		{
			return ERROR;
		}

		folder_name = strtok(NULL, "/");
	}

	return 0;
}

int EXT2Open(const char* device_name, const char* file_name)
{
	unsigned int traget_inode;
	char* path_name = (char*)file_name;
	char* base_name;
	char* folder_name;
	
    assert(device_name);
    assert(file_name);
	
	strcpy(g_dev_name, device_name);
	
	base_name = basename(path_name);
	folder_name = dirname(path_name);

    g_fd = open(device_name, O_RDONLY);
	if (ERROR == g_fd)
	{
		return ERROR;
	}

    if (ERROR == ReadSuperblock())
	{
		close(g_fd);
		return ERROR;	

	}

	if (ERROR == ReadGroupDescriptors())
	{
		close(g_fd);
		return ERROR;	
	}

	if (ERROR == ReadInode(EXT2_ROOT_INO))
	{
		close(g_fd);
		return ERROR;
	}

	/* Find nested file at folders*/
	if ('.' != *folder_name)
	{	
		if (ERROR == ReadEdgeFolder(folder_name, &traget_inode))
		{
			close(g_fd);
			return ERROR;
		}
	}

	if (ERROR == ReadEntry(base_name, &traget_inode))
	{
		close(g_fd);
		return ERROR;
	}

    return traget_inode;
}

int EXT2Read(int target_inode, void* buf, size_t count)
{
	size_t i = 0;
	int written_bytes = 0;

	assert(buf);
	
	if (0 != ReadInode((target_inode)))
	{
		close(g_fd);
		return ERROR;
	}
	PrintInode();
	
	for (i = 0; i < EXT2_NDIR_BLOCKS && 0 != g_inode.i_block[i]; ++i)
	{
		written_bytes += pread(g_fd, ((char*)buf + written_bytes)
			, MIN(count, g_inode.i_size), (g_inode.i_block[i] * g_block_size));
	}

	return written_bytes;
}