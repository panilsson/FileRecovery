/*
 *  linux/include/linux/ext3_fs.h
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/include/linux/minix_fs.h
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#ifndef _LINUX_EXT3_FS_H
#define _LINUX_EXT3_FS_H

#include <linux/types.h>
#include <linux/magic.h>
#include <sys/mount.h>

#if defined(__x86_64__) || defined(__i386__)
typedef unsigned short umode_t;
#endif

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT3FS_DEBUG to produce debug messages
 */
#undef EXT3FS_DEBUG

/*
 * Define EXT3_RESERVATION to reserve data blocks for expanding files
 */
#define EXT3_DEFAULT_RESERVE_BLOCKS     8
/*max window size: 1024(direct blocks) + 3([t,d]indirect blocks) */
#define EXT3_MAX_RESERVE_BLOCKS         1027
#define EXT3_RESERVE_WINDOW_NOT_ALLOCATED 0

/*
 * Debug code
 */
#ifdef EXT3FS_DEBUG
#define ext3_debug(f, a...)						\
	do {								\
		printk (KERN_DEBUG "EXT3-fs DEBUG (%s, %d): %s:",	\
			__FILE__, __LINE__, __func__);		\
		printk (KERN_DEBUG f, ## a);				\
	} while (0)
#else
#define ext3_debug(f, a...)	do {} while (0)
#endif

/*
 * Special inodes numbers
 */
#define	EXT3_BAD_INO		 1	/* Bad blocks inode */
#define EXT3_ROOT_INO		 2	/* Root inode */
#define EXT3_BOOT_LOADER_INO	 5	/* Boot loader inode */
#define EXT3_UNDEL_DIR_INO	 6	/* Undelete directory inode */
#define EXT3_RESIZE_INO		 7	/* Reserved group descriptors inode */
#define EXT3_JOURNAL_INO	 8	/* Journal inode */

/* First non-reserved inode for old ext3 filesystems */
#define EXT3_GOOD_OLD_FIRST_INO	11

/*
 * Maximal count of links to a file
 */
#define EXT3_LINK_MAX		32000

/*
 * Macro-instructions used to manage several block sizes
 */
#define EXT3_MIN_BLOCK_SIZE		1024
#define	EXT3_MAX_BLOCK_SIZE		65536
#define EXT3_MIN_BLOCK_LOG_SIZE		10
#define EXT3_BLOCK_SIZE(s)		(EXT3_MIN_BLOCK_SIZE << (s)->s_log_block_size)
#define	EXT3_ADDR_PER_BLOCK(s)		(EXT3_BLOCK_SIZE(s) / sizeof (__u32))
#define EXT3_BLOCK_SIZE_BITS(s)	((s)->s_log_block_size + 10)
#define EXT3_INODE_SIZE(s)	(((s)->s_rev_level == EXT3_GOOD_OLD_REV) ? \
				 EXT3_GOOD_OLD_INODE_SIZE : \
				 (s)->s_inode_size)
#define EXT3_FIRST_INO(s)	(((s)->s_rev_level == EXT3_GOOD_OLD_REV) ? \
				 EXT3_GOOD_OLD_FIRST_INO : \
				 (s)->s_first_ino)

/*
 * Macro-instructions used to manage fragments
 */
#define EXT3_MIN_FRAG_SIZE		1024
#define	EXT3_MAX_FRAG_SIZE		4096
#define EXT3_MIN_FRAG_LOG_SIZE		  10
#define EXT3_FRAG_SIZE(s)		(EXT3_MIN_FRAG_SIZE << (s)->s_log_frag_size)
#define EXT3_FRAGS_PER_BLOCK(s)	(EXT3_BLOCK_SIZE(s) / EXT3_FRAG_SIZE(s))

/*
 * Structure of a blocks group descriptor
 */
struct ext3_group_desc
{
  __le32bg_block_bitmap_lo;/* Blocks bitmap block */
  __le32bg_inode_bitmap_lo;/* Inodes bitmap block */
  __le32bg_inode_table_lo;/* Inodes table block */
  __le16bg_free_blocks_count_lo;/* Free blocks count */
  __le16bg_free_inodes_count_lo;/* Free inodes count */
  __le16bg_used_dirs_count_lo;/* Directories count */
  __le16bg_flags;/* EXT4_BG_flags (INODE_UNINIT, etc) */
  __le32  bg_exclude_bitmap_lo;   /* Exclude bitmap for snapshots */
  __le16  bg_block_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bbitmap) LE */
  __le16  bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+ibitmap) LE */
  __le16  bg_itable_unused_lo;/* Unused inodes count */
  __le16  bg_checksum;/* crc16(sb_uuid+group+desc) */
  __le32bg_block_bitmap_hi;/* Blocks bitmap block MSB */
  __le32bg_inode_bitmap_hi;/* Inodes bitmap block MSB */
  __le32bg_inode_table_hi;/* Inodes table block MSB */
  __le16bg_free_blocks_count_hi;/* Free blocks count MSB */
  __le16bg_free_inodes_count_hi;/* Free inodes count MSB */
  __le16bg_used_dirs_count_hi;/* Directories count MSB */
  __le16  bg_itable_unused_hi;    /* Unused inodes count MSB */
  __le32  bg_exclude_bitmap_hi;   /* Exclude bitmap block MSB */
  __le16  bg_block_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bbitmap) BE */
  __le16  bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+ibitmap) BE */
  __u32   bg_reserved;
};

/*
 * Macro-instructions used to manage group descriptors
 */
# define EXT3_BLOCKS_PER_GROUP(s)	((s)->s_blocks_per_group)
# define EXT3_DESC_PER_BLOCK(s)		(EXT3_BLOCK_SIZE(s) / sizeof (struct ext3_group_desc))
# define EXT3_INODES_PER_GROUP(s)	((s)->s_inodes_per_group)

/*
 * Constants relative to the data blocks
 */
#define	EXT3_NDIR_BLOCKS		12
#define	EXT3_IND_BLOCK			EXT3_NDIR_BLOCKS
#define	EXT3_DIND_BLOCK			(EXT3_IND_BLOCK + 1)
#define	EXT3_TIND_BLOCK			(EXT3_DIND_BLOCK + 1)
#define	EXT3_N_BLOCKS			(EXT3_TIND_BLOCK + 1)

/*
 * Inode flags
 */
#define	EXT3_SECRM_FL			0x00000001 /* Secure deletion */
#define	EXT3_UNRM_FL			0x00000002 /* Undelete */
#define	EXT3_COMPR_FL			0x00000004 /* Compress file */
#define EXT3_SYNC_FL			0x00000008 /* Synchronous updates */
#define EXT3_IMMUTABLE_FL		0x00000010 /* Immutable file */
#define EXT3_APPEND_FL			0x00000020 /* writes to file may only append */
#define EXT3_NODUMP_FL			0x00000040 /* do not dump file */
#define EXT3_NOATIME_FL			0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#define EXT3_DIRTY_FL			0x00000100
#define EXT3_COMPRBLK_FL		0x00000200 /* One or more compressed clusters */
#define EXT3_NOCOMPR_FL			0x00000400 /* Don't compress */
#define EXT3_ECOMPR_FL			0x00000800 /* Compression error */
/* End compression flags --- maybe not all used */
#define EXT3_INDEX_FL			0x00001000 /* hash-indexed directory */
#define EXT3_IMAGIC_FL			0x00002000 /* AFS directory */
#define EXT3_JOURNAL_DATA_FL		0x00004000 /* file data should be journaled */
#define EXT3_NOTAIL_FL			0x00008000 /* file tail should not be merged */
#define EXT3_DIRSYNC_FL			0x00010000 /* dirsync behaviour (directories only) */
#define EXT3_TOPDIR_FL			0x00020000 /* Top of directory hierarchies*/
#define EXT3_RESERVED_FL		0x80000000 /* reserved for ext3 lib */

#define EXT3_FL_USER_VISIBLE		0x0003DFFF /* User visible flags */
#define EXT3_FL_USER_MODIFIABLE		0x000380FF /* User modifiable flags */

/* Flags that should be inherited by new inodes from their parent. */
#define EXT3_FL_INHERITED (EXT3_SECRM_FL | EXT3_UNRM_FL | EXT3_COMPR_FL |\
			   EXT3_SYNC_FL | EXT3_NODUMP_FL |\
			   EXT3_NOATIME_FL | EXT3_COMPRBLK_FL |\
			   EXT3_NOCOMPR_FL | EXT3_JOURNAL_DATA_FL |\
			   EXT3_NOTAIL_FL | EXT3_DIRSYNC_FL)

/* Flags that are appropriate for regular files (all but dir-specific ones). */
#define EXT3_REG_FLMASK (~(EXT3_DIRSYNC_FL | EXT3_TOPDIR_FL))

/* Flags that are appropriate for non-directories/regular files. */
#define EXT3_OTHER_FLMASK (EXT3_NODUMP_FL | EXT3_NOATIME_FL)

/* Mask out flags that are inappropriate for the given type of inode. */
static inline __u32 ext3_mask_flags(umode_t mode, __u32 flags)
{
	if (S_ISDIR(mode))
		return flags;
	else if (S_ISREG(mode))
		return flags & EXT3_REG_FLMASK;
	else
		return flags & EXT3_OTHER_FLMASK;
}

/* Used to pass group descriptor data when online resize is done */
struct ext3_new_group_input {
	__u32 group;            /* Group number for this data */
	__u32 block_bitmap;     /* Absolute block number of block bitmap */
	__u32 inode_bitmap;     /* Absolute block number of inode bitmap */
	__u32 inode_table;      /* Absolute block number of inode table start */
	__u32 blocks_count;     /* Total number of blocks in this group */
	__u16 reserved_blocks;  /* Number of reserved blocks in this group */
	__u16 unused;
};

/* The struct ext3_new_group_input in kernel space, with free_blocks_count */
struct ext3_new_group_data {
	__u32 group;
	__u32 block_bitmap;
	__u32 inode_bitmap;
	__u32 inode_table;
	__u32 blocks_count;
	__u16 reserved_blocks;
	__u16 unused;
	__u32 free_blocks_count;
};


/*
 * ioctl commands
 */
#define	EXT3_IOC_GETFLAGS		FS_IOC_GETFLAGS
#define	EXT3_IOC_SETFLAGS		FS_IOC_SETFLAGS
#define	EXT3_IOC_GETVERSION		_IOR('f', 3, long)
#define	EXT3_IOC_SETVERSION		_IOW('f', 4, long)
#define EXT3_IOC_GROUP_EXTEND		_IOW('f', 7, unsigned long)
#define EXT3_IOC_GROUP_ADD		_IOW('f', 8,struct ext3_new_group_input)
#define	EXT3_IOC_GETVERSION_OLD		FS_IOC_GETVERSION
#define	EXT3_IOC_SETVERSION_OLD		FS_IOC_SETVERSION
#ifdef CONFIG_JBD_DEBUG
#define EXT3_IOC_WAIT_FOR_READONLY	_IOR('f', 99, long)
#endif
#define EXT3_IOC_GETRSVSZ		_IOR('f', 5, long)
#define EXT3_IOC_SETRSVSZ		_IOW('f', 6, long)

/*
 * ioctl commands in 32 bit emulation
 */
#define EXT3_IOC32_GETFLAGS		FS_IOC32_GETFLAGS
#define EXT3_IOC32_SETFLAGS		FS_IOC32_SETFLAGS
#define EXT3_IOC32_GETVERSION		_IOR('f', 3, int)
#define EXT3_IOC32_SETVERSION		_IOW('f', 4, int)
#define EXT3_IOC32_GETRSVSZ		_IOR('f', 5, int)
#define EXT3_IOC32_SETRSVSZ		_IOW('f', 6, int)
#define EXT3_IOC32_GROUP_EXTEND		_IOW('f', 7, unsigned int)
#ifdef CONFIG_JBD_DEBUG
#define EXT3_IOC32_WAIT_FOR_READONLY	_IOR('f', 99, int)
#endif
#define EXT3_IOC32_GETVERSION_OLD	FS_IOC32_GETVERSION
#define EXT3_IOC32_SETVERSION_OLD	FS_IOC32_SETVERSION


/*
 *  Mount options
 */
struct ext3_mount_options {
	unsigned long s_mount_opt;
	uid_t s_resuid;
	gid_t s_resgid;
	unsigned long s_commit_interval;
#ifdef CONFIG_QUOTA
	int s_jquota_fmt;
	char *s_qf_names[MAXQUOTAS];
#endif
};

/*
 * Structure of an inode on the disk
 */
  struct ext3_inode {
    __le16i_mode;/* File mode */
    __le16i_uid;/* Low 16 bits of Owner Uid */
    __le32i_size_lo;/* Size in bytes */
    __le32i_atime;/* Access time */
    __le32i_ctime;/* Inode Change time */
    __le32i_mtime;/* Modification time */
    __le32i_dtime;/* Deletion Time */
    __le16i_gid;/* Low 16 bits of Group Id */
    __le16i_links_count;/* Links count */
    __le32i_blocks_lo;/* Blocks count */
    __le32i_flags;/* File flags */
    union {
      struct {
	__le32  l_i_version;
      } linux1;
      struct {
	__u32  h_i_translator;
      } hurd1;
      struct {
	__u32  m_i_reserved1;
      } masix1;
    } osd1;/* OS dependent 1 */
    __le32i_block[EXT4_N_BLOCKS];/* Pointers to blocks */
    __le32i_generation;/* File version (for NFS) */
    __le32i_file_acl_lo;/* File ACL */
    __le32i_size_high;
    __le32i_obso_faddr;/* Obsoleted fragment address */
    union {
      struct {
	__le16l_i_blocks_high; /* were l_i_reserved1 */
	__le16l_i_file_acl_high;
	__le16l_i_uid_high;/* these 2 fields */
	__le16l_i_gid_high;/* were reserved2[0] */
	__le16l_i_checksum_lo;/* crc32c(uuid+inum+inode) LE */
	__le16l_i_reserved;
      } linux2;
      struct {
	__le16h_i_reserved1;/* Obsoleted fragment number/size which are removed in ext4 */
	__u16h_i_mode_high;
	__u16h_i_uid_high;
	__u16h_i_gid_high;
	__u32h_i_author;
      } hurd2;
      struct {
	__le16h_i_reserved1;/* Obsoleted fragment number/size which are removed in ext4 */
	__le16m_i_file_acl_high;
	__u32m_i_reserved2[2];
      } masix2;
    } osd2;/* OS dependent 2 */
    __le16i_extra_isize;
    __le16i_checksum_hi;/* crc32c(uuid+inum+inode) BE */
    __le32  i_ctime_extra;  /* extra Change time      (nsec << 2 | epoch) */
    __le32  i_mtime_extra;  /* extra Modification time(nsec << 2 | epoch) */
    __le32  i_atime_extra;  /* extra Access time      (nsec << 2 | epoch) */
    __le32  i_crtime;       /* File Creation time */
    __le32  i_crtime_extra; /* extra FileCreationtime (nsec << 2 | epoch) */
    __le32  i_version_hi;/* high 32 bits for 64-bit version */
    __le32i_projid;/* Project ID */
};

#define i_size_high	i_dir_acl

/*
#define i_translator	osd1.hurd1.h_i_translator
#define i_frag		osd2.hurd2.h_i_frag;
#define i_fsize		osd2.hurd2.h_i_fsize;
#define i_uid_high	osd2.hurd2.h_i_uid_high
#define i_gid_high	osd2.hurd2.h_i_gid_high
#define i_author	osd2.hurd2.h_i_author
*/

/*
 * File system states
 */
#define	EXT3_VALID_FS			0x0001	/* Unmounted cleanly */
#define	EXT3_ERROR_FS			0x0002	/* Errors detected */
#define	EXT3_ORPHAN_FS			0x0004	/* Orphans being recovered */

/*
 * Misc. filesystem flags
 */
#define EXT2_FLAGS_SIGNED_HASH		0x0001  /* Signed dirhash in use */
#define EXT2_FLAGS_UNSIGNED_HASH	0x0002  /* Unsigned dirhash in use */
#define EXT2_FLAGS_TEST_FILESYS		0x0004	/* to test development code */

/*
 * Mount flags
 */
#define EXT3_MOUNT_CHECK		0x00001	/* Do mount-time checks */
/* EXT3_MOUNT_OLDALLOC was there */
#define EXT3_MOUNT_GRPID		0x00004	/* Create files with directory's group */
#define EXT3_MOUNT_DEBUG		0x00008	/* Some debugging messages */
#define EXT3_MOUNT_ERRORS_CONT		0x00010	/* Continue on errors */
#define EXT3_MOUNT_ERRORS_RO		0x00020	/* Remount fs ro on errors */
#define EXT3_MOUNT_ERRORS_PANIC		0x00040	/* Panic on errors */
#define EXT3_MOUNT_MINIX_DF		0x00080	/* Mimics the Minix statfs */
#define EXT3_MOUNT_NOLOAD		0x00100	/* Don't use existing journal*/
#define EXT3_MOUNT_ABORT		0x00200	/* Fatal error detected */
#define EXT3_MOUNT_DATA_FLAGS		0x00C00	/* Mode for data writes: */
#define EXT3_MOUNT_JOURNAL_DATA		0x00400	/* Write data to journal */
#define EXT3_MOUNT_ORDERED_DATA		0x00800	/* Flush data before commit */
#define EXT3_MOUNT_WRITEBACK_DATA	0x00C00	/* No data ordering */
#define EXT3_MOUNT_UPDATE_JOURNAL	0x01000	/* Update the journal format */
#define EXT3_MOUNT_NO_UID32		0x02000  /* Disable 32-bit UIDs */
#define EXT3_MOUNT_XATTR_USER		0x04000	/* Extended user attributes */
#define EXT3_MOUNT_POSIX_ACL		0x08000	/* POSIX Access Control Lists */
#define EXT3_MOUNT_RESERVATION		0x10000	/* Preallocation */
#define EXT3_MOUNT_BARRIER		0x20000 /* Use block barriers */
#define EXT3_MOUNT_QUOTA		0x80000 /* Some quota option set */
#define EXT3_MOUNT_USRQUOTA		0x100000 /* "old" user quota */
#define EXT3_MOUNT_GRPQUOTA		0x200000 /* "old" group quota */
#define EXT3_MOUNT_DATA_ERR_ABORT	0x400000 /* Abort on file data write
						  * error in ordered mode */

/* Compatibility, for having both ext2_fs.h and ext3_fs.h included at once */
#ifndef _LINUX_EXT2_FS_H
#define clear_opt(o, opt)		o &= ~EXT3_MOUNT_##opt
#define set_opt(o, opt)			o |= EXT3_MOUNT_##opt
#define test_opt(sb, opt)		(EXT3_SB(sb)->s_mount_opt & \
					 EXT3_MOUNT_##opt)
#else
#define EXT2_MOUNT_NOLOAD		EXT3_MOUNT_NOLOAD
#define EXT2_MOUNT_ABORT		EXT3_MOUNT_ABORT
#define EXT2_MOUNT_DATA_FLAGS		EXT3_MOUNT_DATA_FLAGS
#endif

#define ext3_set_bit			__set_bit_le
#define ext3_set_bit_atomic		ext2_set_bit_atomic
#define ext3_clear_bit			__clear_bit_le
#define ext3_clear_bit_atomic		ext2_clear_bit_atomic
#define ext3_test_bit			test_bit_le
#define ext3_find_next_zero_bit		find_next_zero_bit_le

/*
 * Maximal mount counts between two filesystem checks
 */
#define EXT3_DFL_MAX_MNT_COUNT		20	/* Allow 20 mounts */
#define EXT3_DFL_CHECKINTERVAL		0	/* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#define EXT3_ERRORS_CONTINUE		1	/* Continue execution */
#define EXT3_ERRORS_RO			2	/* Remount fs read-only */
#define EXT3_ERRORS_PANIC		3	/* Panic */
#define EXT3_ERRORS_DEFAULT		EXT3_ERRORS_CONTINUE

/*
 * Structure of the super block
 */
struct ext3_super_block {
/*00*/	__le32	s_inodes_count;		/* Inodes count */
	__le32	s_blocks_count;		/* Blocks count */
	__le32	s_r_blocks_count;	/* Reserved blocks count */
	__le32	s_free_blocks_count;	/* Free blocks count */
/*10*/	__le32	s_free_inodes_count;	/* Free inodes count */
	__le32	s_first_data_block;	/* First Data Block */
	__le32	s_log_block_size;	/* Block size */
	__le32	s_log_frag_size;	/* Fragment size */
/*20*/	__le32	s_blocks_per_group;	/* # Blocks per group */
	__le32	s_frags_per_group;	/* # Fragments per group */
	__le32	s_inodes_per_group;	/* # Inodes per group */
	__le32	s_mtime;		/* Mount time */
/*30*/	__le32	s_wtime;		/* Write time */
	__le16	s_mnt_count;		/* Mount count */
	__le16	s_max_mnt_count;	/* Maximal mount count */
	__le16	s_magic;		/* Magic signature */
	__le16	s_state;		/* File system state */
	__le16	s_errors;		/* Behaviour when detecting errors */
	__le16	s_minor_rev_level;	/* minor revision level */
/*40*/	__le32	s_lastcheck;		/* time of last check */
	__le32	s_checkinterval;	/* max. time between checks */
	__le32	s_creator_os;		/* OS */
	__le32	s_rev_level;		/* Revision level */
/*50*/	__le16	s_def_resuid;		/* Default uid for reserved blocks */
	__le16	s_def_resgid;		/* Default gid for reserved blocks */
	/*
	 * These fields are for EXT3_DYNAMIC_REV superblocks only.
	 *
	 * Note: the difference between the compatible feature set and
	 * the incompatible feature set is that if there is a bit set
	 * in the incompatible feature set that the kernel doesn't
	 * know about, it should refuse to mount the filesystem.
	 *
	 * e2fsck's requirements are more strict; if it doesn't know
	 * about a feature in either the compatible or incompatible
	 * feature set, it must abort and not try to meddle with
	 * things it doesn't understand...
	 */
	__le32	s_first_ino;		/* First non-reserved inode */
	__le16   s_inode_size;		/* size of inode structure */
	__le16	s_block_group_nr;	/* block group # of this superblock */
	__le32	s_feature_compat;	/* compatible feature set */
/*60*/	__le32	s_feature_incompat;	/* incompatible feature set */
	__le32	s_feature_ro_compat;	/* readonly-compatible feature set */
/*68*/	__u8	s_uuid[16];		/* 128-bit uuid for volume */
/*78*/	char	s_volume_name[16];	/* volume name */
/*88*/	char	s_last_mounted[64];	/* directory where last mounted */
/*C8*/	__le32	s_algorithm_usage_bitmap; /* For compression */
	/*
	 * Performance hints.  Directory preallocation should only
	 * happen if the EXT3_FEATURE_COMPAT_DIR_PREALLOC flag is on.
	 */
	__u8	s_prealloc_blocks;	/* Nr of blocks to try to preallocate*/
	__u8	s_prealloc_dir_blocks;	/* Nr to preallocate for dirs */
	__le16	s_reserved_gdt_blocks;	/* Per group desc for online growth */
	/*
	 * Journaling support valid if EXT3_FEATURE_COMPAT_HAS_JOURNAL set.
	 */
/*D0*/	__u8	s_journal_uuid[16];	/* uuid of journal superblock */
/*E0*/	__le32	s_journal_inum;		/* inode number of journal file */
	__le32	s_journal_dev;		/* device number of journal file */
	__le32	s_last_orphan;		/* start of list of inodes to delete */
	__le32	s_hash_seed[4];		/* HTREE hash seed */
	__u8	s_def_hash_version;	/* Default hash version to use */
	__u8	s_reserved_char_pad;
	__u16	s_reserved_word_pad;
	__le32	s_default_mount_opts;
	__le32	s_first_meta_bg;	/* First metablock block group */
	__le32	s_mkfs_time;		/* When the filesystem was created */
	__le32	s_jnl_blocks[17];	/* Backup of the journal inode */
	/* 64bit support valid if EXT4_FEATURE_COMPAT_64BIT */
/*150*/	__le32	s_blocks_count_hi;	/* Blocks count */
	__le32	s_r_blocks_count_hi;	/* Reserved blocks count */
	__le32	s_free_blocks_count_hi;	/* Free blocks count */
	__le16	s_min_extra_isize;	/* All inodes have at least # bytes */
	__le16	s_want_extra_isize; 	/* New inodes should reserve # bytes */
	__le32	s_flags;		/* Miscellaneous flags */
	__le16  s_raid_stride;		/* RAID stride */
	__le16  s_mmp_interval;         /* # seconds to wait in MMP checking */
	__le64  s_mmp_block;            /* Block for multi-mount protection */
	__le32  s_raid_stripe_width;    /* blocks on all data disks (N*stride)*/
	__u8	s_log_groups_per_flex;  /* FLEX_BG group size */
	__u8	s_reserved_char_pad2;
	__le16  s_reserved_pad;
	__u32   s_reserved[162];        /* Padding to the end of the block */
};

/* Assume that user mode programs are passing in an ext3fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#define EXT3_SB(sb)	(sb)

#define NEXT_ORPHAN(inode) EXT3_I(inode)->i_dtime

/*
 * Codes for operating systems
 */
#define EXT3_OS_LINUX		0
#define EXT3_OS_HURD		1
#define EXT3_OS_MASIX		2
#define EXT3_OS_FREEBSD		3
#define EXT3_OS_LITES		4

/*
 * Revision levels
 */
#define EXT3_GOOD_OLD_REV	0	/* The good old (original) format */
#define EXT3_DYNAMIC_REV	1	/* V2 format w/ dynamic inode sizes */

#define EXT3_CURRENT_REV	EXT3_GOOD_OLD_REV
#define EXT3_MAX_SUPP_REV	EXT3_DYNAMIC_REV

#define EXT3_GOOD_OLD_INODE_SIZE 128

/*
 * Feature set definitions
 */

#define EXT3_HAS_COMPAT_FEATURE(sb,mask)			\
	( EXT3_SB(sb)->s_es->s_feature_compat & cpu_to_le32(mask) )
#define EXT3_HAS_RO_COMPAT_FEATURE(sb,mask)			\
	( EXT3_SB(sb)->s_es->s_feature_ro_compat & cpu_to_le32(mask) )
#define EXT3_HAS_INCOMPAT_FEATURE(sb,mask)			\
	( EXT3_SB(sb)->s_es->s_feature_incompat & cpu_to_le32(mask) )
#define EXT3_SET_COMPAT_FEATURE(sb,mask)			\
	EXT3_SB(sb)->s_es->s_feature_compat |= cpu_to_le32(mask)
#define EXT3_SET_RO_COMPAT_FEATURE(sb,mask)			\
	EXT3_SB(sb)->s_es->s_feature_ro_compat |= cpu_to_le32(mask)
#define EXT3_SET_INCOMPAT_FEATURE(sb,mask)			\
	EXT3_SB(sb)->s_es->s_feature_incompat |= cpu_to_le32(mask)
#define EXT3_CLEAR_COMPAT_FEATURE(sb,mask)			\
	EXT3_SB(sb)->s_es->s_feature_compat &= ~cpu_to_le32(mask)
#define EXT3_CLEAR_RO_COMPAT_FEATURE(sb,mask)			\
	EXT3_SB(sb)->s_es->s_feature_ro_compat &= ~cpu_to_le32(mask)
#define EXT3_CLEAR_INCOMPAT_FEATURE(sb,mask)			\
	EXT3_SB(sb)->s_es->s_feature_incompat &= ~cpu_to_le32(mask)

#define EXT3_FEATURE_COMPAT_DIR_PREALLOC	0x0001
#define EXT3_FEATURE_COMPAT_IMAGIC_INODES	0x0002
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL		0x0004
#define EXT3_FEATURE_COMPAT_EXT_ATTR		0x0008
#define EXT3_FEATURE_COMPAT_RESIZE_INODE	0x0010
#define EXT3_FEATURE_COMPAT_DIR_INDEX		0x0020

#define EXT3_FEATURE_RO_COMPAT_SPARSE_SUPER	0x0001
#define EXT3_FEATURE_RO_COMPAT_LARGE_FILE	0x0002
#define EXT3_FEATURE_RO_COMPAT_BTREE_DIR	0x0004

#define EXT3_FEATURE_INCOMPAT_COMPRESSION	0x0001
#define EXT3_FEATURE_INCOMPAT_FILETYPE		0x0002
#define EXT3_FEATURE_INCOMPAT_RECOVER		0x0004 /* Needs recovery */
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV	0x0008 /* Journal device */
#define EXT3_FEATURE_INCOMPAT_META_BG		0x0010

#define EXT3_FEATURE_COMPAT_SUPP	EXT2_FEATURE_COMPAT_EXT_ATTR
#define EXT3_FEATURE_INCOMPAT_SUPP	(EXT3_FEATURE_INCOMPAT_FILETYPE| \
					 EXT3_FEATURE_INCOMPAT_RECOVER| \
					 EXT3_FEATURE_INCOMPAT_META_BG)
#define EXT3_FEATURE_RO_COMPAT_SUPP	(EXT3_FEATURE_RO_COMPAT_SPARSE_SUPER| \
					 EXT3_FEATURE_RO_COMPAT_LARGE_FILE| \
					 EXT3_FEATURE_RO_COMPAT_BTREE_DIR)

/*
 * Default values for user and/or group using reserved blocks
 */
#define	EXT3_DEF_RESUID		0
#define	EXT3_DEF_RESGID		0

/*
 * Default mount options
 */
#define EXT3_DEFM_DEBUG		0x0001
#define EXT3_DEFM_BSDGROUPS	0x0002
#define EXT3_DEFM_XATTR_USER	0x0004
#define EXT3_DEFM_ACL		0x0008
#define EXT3_DEFM_UID16		0x0010
#define EXT3_DEFM_JMODE		0x0060
#define EXT3_DEFM_JMODE_DATA	0x0020
#define EXT3_DEFM_JMODE_ORDERED	0x0040
#define EXT3_DEFM_JMODE_WBACK	0x0060

/*
 * Structure of a directory entry
 */
#define EXT3_NAME_LEN 255

struct ext3_dir_entry {
	__le32	inode;			/* Inode number */
	__le16	rec_len;		/* Directory entry length */
	__le16	name_len;		/* Name length */
	char	name[EXT3_NAME_LEN];	/* File name */
};

/*
 * The new version of the directory entry.  Since EXT3 structures are
 * stored in intel byte order, and the name_len field could never be
 * bigger than 255 chars, it's safe to reclaim the extra byte for the
 * file_type field.
 */
struct ext3_dir_entry_2 {
	__le32	inode;			/* Inode number */
	__le16	rec_len;		/* Directory entry length */
	__u8	name_len;		/* Name length */
	__u8	file_type;
	char	name[EXT3_NAME_LEN];	/* File name */
};

/*
 * Ext3 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#define EXT3_FT_UNKNOWN		0
#define EXT3_FT_REG_FILE	1
#define EXT3_FT_DIR		2
#define EXT3_FT_CHRDEV		3
#define EXT3_FT_BLKDEV		4
#define EXT3_FT_FIFO		5
#define EXT3_FT_SOCK		6
#define EXT3_FT_SYMLINK		7

#define EXT3_FT_MAX		8

/*
 * EXT3_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#define EXT3_DIR_PAD			4
#define EXT3_DIR_ROUND			(EXT3_DIR_PAD - 1)
#define EXT3_DIR_REC_LEN(name_len)	(((name_len) + 8 + EXT3_DIR_ROUND) & \
					 ~EXT3_DIR_ROUND)
#define EXT3_MAX_REC_LEN		((1<<16)-1)

///*
// * Tests against MAX_REC_LEN etc were put in place for 64k block
// * sizes; if that is not possible on this arch, we can skip
// * those tests and speed things up.
// */
//static inline unsigned ext3_rec_len_from_disk(__le16 dlen)
//{
//	unsigned len = le16_to_cpu(dlen);
//
//#if (PAGE_CACHE_SIZE >= 65536)
//	if (len == EXT3_MAX_REC_LEN)
//		return 1 << 16;
//#endif
//	return len;
//}
//
//static inline __le16 ext3_rec_len_to_disk(unsigned len)
//{
//#if (PAGE_CACHE_SIZE >= 65536)
//	if (len == (1 << 16))
//		return cpu_to_le16(EXT3_MAX_REC_LEN);
//	else if (len > (1 << 16))
//		BUG();
//#endif
//	return cpu_to_le16(len);
//}

/*
 * Hash Tree Directory indexing
 * (c) Daniel Phillips, 2001
 */

#define is_dx(dir) (EXT3_HAS_COMPAT_FEATURE(dir->i_sb, \
				      EXT3_FEATURE_COMPAT_DIR_INDEX) && \
		      (EXT3_I(dir)->i_flags & EXT3_INDEX_FL))
#define EXT3_DIR_LINK_MAX(dir) (!is_dx(dir) && (dir)->i_nlink >= EXT3_LINK_MAX)
#define EXT3_DIR_LINK_EMPTY(dir) ((dir)->i_nlink == 2 || (dir)->i_nlink == 1)

/* Legal values for the dx_root hash_version field: */

#define DX_HASH_LEGACY		0
#define DX_HASH_HALF_MD4	1
#define DX_HASH_TEA		2
#define DX_HASH_LEGACY_UNSIGNED	3
#define DX_HASH_HALF_MD4_UNSIGNED	4
#define DX_HASH_TEA_UNSIGNED		5

#endif	/* _LINUX_EXT3_FS_H */
