/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ext2.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:27:18 by maroy             #+#    #+#             */
/*   Updated: 2024/09/11 01:32:05 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/ScopeGuard.h>
#include <FROG/StringView.h>
#include <kernel/FS/Ext2.h>

#define EXT2_DEBUG_PRINT 1

namespace Kernel {

namespace Ext2::Enum {

enum State {
	VALID_FS = 1,
	ERROR_FS = 2,
};

enum Errors {
	ERRORS_CONTINUE = 1,
	ERRORS_RO = 2,
	ERRORS_PANIC = 3,
};

enum CreatorOS {
	OS_LINUX = 0,
	OS_HURD = 1,
	OS_MASIX = 2,
	OS_FREEBSD = 3,
	OS_LITES = 4,
};

enum RevLevel {
	GOOD_OLD_REV = 0,
	DYNAMIC_REV = 1,
};

enum FeatureCompat {
	FEATURE_COMPAT_DIR_PREALLOC = 0x0001,
	FEATURE_COMPAT_IMAGIC_INODES = 0x0002,
	FEATURE_COMPAT_HAS_JOURNAL = 0x0004,
	FEATURE_COMPAT_EXT_ATTR = 0x0008,
	FEATURE_COMPAT_RESIZE_INO = 0x0010,
	FEATURE_COMPAT_DIR_INDEX = 0x0020,
};

enum FeaturesIncompat {
	FEATURE_INCOMPAT_COMPRESSION = 0x0001,
	FEATURE_INCOMPAT_FILETYPE = 0x0002,
	FEATURE_INCOMPAT_RECOVER = 0x0004,
	FEATURE_INCOMPAT_JOURNAL_DEV = 0x0008,
	FEATURE_INCOMPAT_META_BG = 0x0010,
};

enum FeaturesRoCompat {
	FEATURE_RO_COMPAT_SPARSE_SUPER = 0x0001,
	FEATURE_RO_COMPAT_LARGE_FILE = 0x0002,
	FEATURE_RO_COMPAT_BTREE_DIR = 0x0004,
};

enum AlgoBitmap {
	LZV1_ALG = 0,
	LZRW3A_ALG = 1,
	GZIP_ALG = 2,
	BZIP2_ALG = 3,
	LZO_ALG = 4,
};

enum ReservedInodes {
	BAD_INO = 1,
	ROOT_INO = 2,
	ACL_IDX_INO = 3,
	ACL_DATA_INO = 4,
	BOOT_LOADER_INO = 5,
	UNDEL_DIR_INO = 6,
};

enum InodeMode {
	// -- file format --
	IFSOCK = 0xC000,
	IFLNK = 0xA000,
	IFREG = 0x8000,
	IFBLK = 0x6000,
	IFDIR = 0x4000,
	IFCHR = 0x2000,
	IFIFO = 0x1000,

	// -- process execution user/group override --
	ISUID = 0x0800,
	ISGID = 0x0400,
	ISVTX = 0x0200,

	// -- access rights --
	IRUSR = 0x0100,
	IWUSR = 0x0080,
	IXUSR = 0x0040,
	IRGRP = 0x0020,
	IWGRP = 0x0010,
	IXGRP = 0x0008,
	IROTH = 0x0004,
	IWOTH = 0x0002,
	IXOTH = 0x0001,
};

enum InodeFlags {
	SECRM_FL = 0x00000001,
	UNRM_FL = 0x00000002,
	COMPR_FL = 0x00000004,
	SYNC_FL = 0x00000008,
	IMMUTABLE_FL = 0x00000010,
	APPEND_FL = 0x00000020,
	NODUMP_FL = 0x00000040,
	NOATIME_FL = 0x00000080,
	// -- Reserved for compression usage --
	DIRTY_FL = 0x00000100,
	COMPRBLK_FL = 0x00000200,
	NOCOMPR_FL = 0x00000400,
	ECOMPR_FL = 0x00000800,
	// -- End of compression flags --
	BTREE_FL = 0x00001000,
	INDEX_FL = 0x00001000,
	IMAGIC_FL = 0x00002000,
	JOURNAL_DATA_FL = 0x00004000,
	RESERVED_FL = 0x80000000,
};

} // namespace Ext2::Enum

FROG::ErrorOr<FROG::RefPtr<Inode>> Ext2Inode::create(Ext2FS &fs, uint32_t inode, FROG::StringView name) {
	Ext2::Inode ext2_inode = TRY(fs.read_inode(inode));
	Ext2Inode  *result = new Ext2Inode(fs, ext2_inode, name, inode);
	if (result == nullptr) return FROG::Error::from_errno(ENOMEM);
	return FROG::RefPtr<Inode>::adopt(result);
}

FROG::ErrorOr<uint32_t> Ext2Inode::data_block_index(uint32_t asked_data_block) {
	uint32_t data_blocks_count = m_inode.blocks / (2 << m_fs.superblock().log_block_size);
	uint32_t blocks_per_array = (1024 << m_fs.superblock().log_block_size) / sizeof(uint32_t);

	if (asked_data_block >= data_blocks_count)
		return FROG::Error::from_c_string("Ext2: no such block");

	// Direct block
	if (asked_data_block < 12) {
		uint32_t block = m_inode.block[asked_data_block];
		if (block == 0) return FROG::Error::from_errno(EIO);
		return block;
	}

	asked_data_block -= 12;

	// Singly indirect block
	if (asked_data_block < blocks_per_array) {
		if (m_inode.block[12] == 0) return FROG::Error::from_errno(EIO);
		auto     block_array = TRY(m_fs.read_block(m_inode.block[12]));
		uint32_t block = ((uint32_t *) block_array.data())[asked_data_block];
		if (block == 0) return FROG::Error::from_errno(EIO);
		return block;
	}

	asked_data_block -= blocks_per_array;

	// Doubly indirect blocks
	if (asked_data_block < blocks_per_array * blocks_per_array) {
		auto singly_indirect_array = TRY(m_fs.read_block(m_inode.block[13]));
		uint32_t direct_block = ((uint32_t *) singly_indirect_array.data())[asked_data_block / blocks_per_array];
		if (direct_block == 0) return FROG::Error::from_errno(EIO);
		auto     block_array = TRY(m_fs.read_block(direct_block));
		uint32_t block = ((uint32_t *) block_array.data())[asked_data_block % blocks_per_array];
		if (block == 0) return FROG::Error::from_errno(EIO);
		return block;
	}

	asked_data_block -= blocks_per_array * blocks_per_array;

	// Triply indirect blocks
	if (asked_data_block < blocks_per_array * blocks_per_array * blocks_per_array) {
		auto doubly_indirect_array = TRY(m_fs.read_block(m_inode.block[14]));
		uint32_t singly_indirect_block = ((uint32_t *) doubly_indirect_array.data())[asked_data_block / (blocks_per_array * blocks_per_array)];
		if (singly_indirect_block == 0) return FROG::Error::from_errno(EIO);
		auto singly_indirect_array = TRY(m_fs.read_block(singly_indirect_block));
		uint32_t direct_block = ((uint32_t *) singly_indirect_array.data())[(asked_data_block / blocks_per_array) % blocks_per_array];
		if (direct_block == 0) return FROG::Error::from_errno(EIO);
		auto     block_array = TRY(m_fs.read_block(direct_block));
		uint32_t block = ((uint32_t *) block_array.data())[asked_data_block % blocks_per_array];
		if (block == 0) return FROG::Error::from_errno(EIO);
		return block;
	}

	ASSERT(false);
}

FROG::ErrorOr<size_t> Ext2Inode::read(size_t offset, void *buffer, size_t count) {
	if (ifdir()) return FROG::Error::from_errno(EISDIR);

	if (offset >= m_inode.size) return 0;
	if (offset + count > m_inode.size) count = m_inode.size - offset;

	const uint32_t block_size = 1024 << m_fs.superblock().log_block_size;

	const uint32_t first_block = offset / block_size;
	const uint32_t last_block = FROG::Math::div_round_up<uint32_t>(offset + count, block_size);

	size_t n_read = 0;

	for (uint32_t block = first_block; block < last_block; block++) {
		uint32_t block_index = TRY(data_block_index(block));
		auto     block_data = TRY(m_fs.read_block(block_index));
		ASSERT(block_data.size() == block_size);

		uint32_t copy_offset = (offset + n_read) % block_size;
		uint32_t to_copy = FROG::Math::min<uint32_t>(block_size - copy_offset, count - n_read);
		memcpy((uint8_t *) buffer + n_read, block_data.data() + copy_offset, to_copy);

		n_read += to_copy;
	}

	return n_read;
}

FROG::ErrorOr<FROG::RefPtr<Inode>> Ext2Inode::directory_find_impl(FROG::StringView file_name) {
	if (!ifdir()) return FROG::Error::from_errno(ENOTDIR);

	uint32_t data_block_count = m_inode.blocks / (2 << m_fs.superblock().log_block_size);

	for (uint32_t i = 0; i < data_block_count; i++) {
		uint32_t block_index = TRY(data_block_index(i));
		auto     block_data = TRY(m_fs.read_block(block_index));

		const uint8_t *block_data_end = block_data.data() + block_data.size();
		const uint8_t *entry_addr = block_data.data();

		while (entry_addr < block_data_end) {
			const auto      &entry = *(const Ext2::LinkedDirectoryEntry *) entry_addr;
			FROG::StringView entry_name(entry.name, entry.name_len);
			if (entry.inode && entry_name == file_name)
				return TRY(Ext2Inode::create(m_fs, entry.inode, entry.name));
			entry_addr += entry.rec_len;
		}
	}

	return FROG::Error::from_errno(ENOENT);
}

FROG::ErrorOr<FROG::Vector<FROG::RefPtr<Inode>>> Ext2Inode::directory_inodes_impl() {
	if (!ifdir()) return FROG::Error::from_errno(ENOTDIR);

	uint32_t data_block_count = m_inode.blocks / (2 << m_fs.superblock().log_block_size);

	FROG::Vector<FROG::RefPtr<Inode>> inodes;

	for (uint32_t i = 0; i < data_block_count; i++) {
		uint32_t block_index = TRY(data_block_index(i));
		auto     block_data = TRY(m_fs.read_block(block_index));

		const uint8_t *block_data_end = block_data.data() + block_data.size();
		const uint8_t *entry_addr = block_data.data();
		while (entry_addr < block_data_end) {
			const auto &entry = *(const Ext2::LinkedDirectoryEntry *) entry_addr;
			if (entry.inode) {
				FROG::StringView entry_name(entry.name, entry.name_len);
				auto             inode = TRY(Ext2Inode::create(m_fs, entry.inode, entry_name));
				TRY(inodes.push_back(inode));
			}
			entry_addr += entry.rec_len;
		}
	}

	return inodes;
}

bool Ext2Inode::operator==(const Inode &other) const {
	if (type() != other.type()) return false;

	Ext2Inode &ext2_other = (Ext2Inode &) other;
	if (&m_fs != &ext2_other.m_fs) return false;
	return index() == ext2_other.index();
}

FROG::ErrorOr<Ext2FS *> Ext2FS::create(StorageDevice::Partition &partition) {
	Ext2FS *ext2fs = new Ext2FS(partition);
	if (ext2fs == nullptr) return FROG::Error::from_errno(ENOMEM);
	TRY(ext2fs->initialize_superblock());
	TRY(ext2fs->initialize_block_group_descriptors());
	TRY(ext2fs->initialize_root_inode());
	return ext2fs;
}

FROG::ErrorOr<void> Ext2FS::initialize_superblock() {
	const uint32_t sector_size = m_partition.device().sector_size();
	ASSERT(1024 % sector_size == 0);

	// Read superblock from disk
	{
		uint8_t *superblock_buffer = (uint8_t *) kmalloc(1024);
		if (superblock_buffer == nullptr) FROG::Error::from_errno(ENOMEM);
		FROG::ScopeGuard _([superblock_buffer] { kfree(superblock_buffer); });

		uint32_t lba = 1024 / sector_size;
		uint32_t sector_count = 1024 / sector_size;

		TRY(m_partition.read_sectors(lba, sector_count, superblock_buffer));

		memcpy(&m_superblock, superblock_buffer, sizeof(Ext2::Superblock));
	}

	if (m_superblock.magic != 0xEF53) return FROG::Error::from_c_string("Not a ext2 filesystem");

	if (m_superblock.rev_level < 1) {
		memset(m_superblock.__extension_start, 0, sizeof(Ext2::Superblock) - offsetof(Ext2::Superblock, Ext2::Superblock::__extension_start));
		m_superblock.first_ino = 11;
		m_superblock.inode_size = 128;
	}

	ASSERT(!(m_superblock.feature_incompat & Ext2::Enum::FEATURE_INCOMPAT_COMPRESSION));
	// ASSERT(!(m_superblock.feature_incompat & Ext2::Enum::FEATURE_INCOMPAT_FILETYPE));
	ASSERT(!(m_superblock.feature_incompat & Ext2::Enum::FEATURE_INCOMPAT_JOURNAL_DEV));
	ASSERT(!(m_superblock.feature_incompat & Ext2::Enum::FEATURE_INCOMPAT_META_BG));
	ASSERT(!(m_superblock.feature_incompat & Ext2::Enum::FEATURE_INCOMPAT_RECOVER));

#if EXT2_DEBUG_PRINT
	dprintln("EXT2");
	dprintln("  inodes        {}", m_superblock.inodes_count);
	dprintln("  blocks        {}", m_superblock.blocks_count);
	dprintln("  version       {}.{}", m_superblock.rev_level, m_superblock.minor_rev_level);
	dprintln("  first data at {}", m_superblock.first_data_block);
	dprintln("  block size    {}", 1024 << m_superblock.log_block_size);
#endif

	return {};
}

FROG::ErrorOr<void> Ext2FS::initialize_block_group_descriptors() {
	const uint32_t sector_size = m_partition.device().sector_size();
	const uint32_t block_size = 1024 << m_superblock.log_block_size;
	const uint32_t sectors_per_block = block_size / sector_size;
	ASSERT(block_size % sector_size == 0);

	uint32_t number_of_block_groups =
	    FROG::Math::div_round_up(m_superblock.inodes_count, m_superblock.inodes_per_group);
	uint32_t number_of_block_groups_check =
	    FROG::Math::div_round_up(m_superblock.blocks_count, m_superblock.blocks_per_group);
	if (number_of_block_groups != number_of_block_groups_check)
		return FROG::Error::from_c_string("Ambiguous number of blocks");

	uint32_t block_group_descriptor_table_block = m_superblock.first_data_block + 1;
	uint32_t block_group_descriptor_table_sector_count = FROG::Math::div_round_up(32u * number_of_block_groups, sector_size);

	uint8_t *block_group_descriptor_table_buffer = (uint8_t *) kmalloc(block_group_descriptor_table_sector_count * sector_size);
	if (block_group_descriptor_table_buffer == nullptr) return FROG::Error::from_errno(ENOMEM);
	FROG::ScopeGuard _(
	    [block_group_descriptor_table_buffer] { kfree(block_group_descriptor_table_buffer); });

	TRY(m_partition.read_sectors(block_group_descriptor_table_block * sectors_per_block, block_group_descriptor_table_sector_count, block_group_descriptor_table_buffer));
	TRY(m_block_group_descriptors.resize(number_of_block_groups));

	for (uint32_t i = 0; i < number_of_block_groups; i++) {
		memcpy(&m_block_group_descriptors[i], block_group_descriptor_table_buffer + 32u * i, sizeof(Ext2::BlockGroupDescriptor));

#if EXT2_DEBUG_PRINT
		dprintln("block group descriptor {}", i);
		dprintln("  block bitmap   {}", m_block_group_descriptors[i].block_bitmap);
		dprintln("  inode bitmap   {}", m_block_group_descriptors[i].inode_bitmap);
		dprintln("  inode table    {}", m_block_group_descriptors[i].inode_table);
		dprintln("  unalloc blocks {}", m_block_group_descriptors[i].free_blocks_count);
		dprintln("  unalloc inodes {}", m_block_group_descriptors[i].free_inodes_count);
#endif
	}

	return {};
}

FROG::ErrorOr<void> Ext2FS::initialize_root_inode() {
	m_root_inode = TRY(Ext2Inode::create(*this, Ext2::Enum::ROOT_INO, ""));

#if EXT2_DEBUG_PRINT
	dprintln("root inode:");
	dprintln("  created  {}", ext2_root_inode().ctime);
	dprintln("  modified {}", ext2_root_inode().mtime);
	dprintln("  accessed {}", ext2_root_inode().atime);
#endif
	return {};
}

FROG::ErrorOr<Ext2::Inode> Ext2FS::read_inode(uint32_t inode) {
	uint32_t block_size = 1024 << m_superblock.log_block_size;

	uint32_t inode_block_group = (inode - 1) / m_superblock.inodes_per_group;
	uint32_t local_inode_index = (inode - 1) % m_superblock.inodes_per_group;

	uint32_t inode_table_offset_blocks = (local_inode_index * m_superblock.inode_size) / block_size;
	uint32_t inode_block_offset = (local_inode_index * m_superblock.inode_size) % block_size;

	uint32_t inode_block = m_block_group_descriptors[inode_block_group].inode_table + inode_table_offset_blocks;

	auto        inode_block_buffer = TRY(read_block(inode_block));
	Ext2::Inode ext2_inode;
	memcpy(&ext2_inode, inode_block_buffer.data() + inode_block_offset, sizeof(Ext2::Inode));
	return ext2_inode;
}

FROG::ErrorOr<FROG::Vector<uint8_t>> Ext2FS::read_block(uint32_t block) {
	const uint32_t sector_size = m_partition.device().sector_size();
	uint32_t       block_size = 1024 << m_superblock.log_block_size;
	ASSERT(block_size % sector_size == 0);
	uint32_t sectors_per_block = block_size / sector_size;

	FROG::Vector<uint8_t> block_buffer;
	TRY(block_buffer.resize(block_size));

	TRY(m_partition.read_sectors(block * sectors_per_block, sectors_per_block, block_buffer.data()));

	return block_buffer;
}

const Ext2::Inode &Ext2FS::ext2_root_inode() const {
	return reinterpret_cast<const Ext2Inode *>(m_root_inode.ptr())->m_inode;
}

} // namespace Kernel