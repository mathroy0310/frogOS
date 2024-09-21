/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Process.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:06:34 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 00:22:40 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/StringView.h>
#include <kernel/FS/VirtualFileSystem.h>
#include <kernel/LockGuard.h>
#include <kernel/Process.h>
#include <kernel/Scheduler.h>

#include <fcntl.h>

namespace Kernel {

FROG::ErrorOr<FROG::RefPtr<Process>> Process::create_kernel(entry_t entry, void *data) {
	static pid_t next_pid = 1;
	auto         process = TRY(FROG::RefPtr<Process>::create(next_pid++));
	TRY(process->m_working_directory.push_back('/'));
	TRY(process->add_thread(entry, data));
	return process;
}

FROG::ErrorOr<void> Process::add_thread(entry_t entry, void *data) {
	LockGuard _(m_lock);

	auto thread = TRY(Thread::create(entry, data, this));
	TRY(m_threads.push_back(thread));
	if (auto res = Scheduler::get().add_thread(thread); res.is_error()) {
		m_threads.pop_back();
		return res;
	}
	return {};
}

void Process::on_thread_exit(Thread &thread) {
	LockGuard _(m_lock);
	dprintln("thread {} exit", thread.tid());
	for (size_t i = 0; i < m_threads.size(); i++)
		if (m_threads[i].ptr() == &thread) m_threads.remove(i);
}

FROG::ErrorOr<int> Process::open(FROG::StringView path, int flags) {
	LockGuard _(m_lock);

	if (flags != O_RDONLY) return FROG::Error::from_errno(ENOTSUP);

	FROG::String absolute_path = TRY(absolute_path_of(path));

	auto file = TRY(VirtualFileSystem::get().file_from_absolute_path(absolute_path));

	int   fd = TRY(get_free_fd());
	auto &open_file_description = m_open_files[fd];
	open_file_description.inode = file.inode;
	open_file_description.path = FROG::move(file.canonical_path);
	open_file_description.offset = 0;
	open_file_description.flags = flags;

	return fd;
}

FROG::ErrorOr<void> Process::close(int fd) {
	LockGuard _(m_lock);

	TRY(validate_fd(fd));
	auto &open_file_description = this->open_file_description(fd);
	open_file_description.inode = nullptr;
	return {};
}

FROG::ErrorOr<size_t> Process::read(int fd, void *buffer, size_t count) {
	LockGuard _(m_lock);

	TRY(validate_fd(fd));
	auto &open_fd = open_file_description(fd);
	if (!(open_fd.flags & O_RDONLY)) return FROG::Error::from_errno(EBADF);
	size_t n_read = TRY(open_fd.inode->read(open_fd.offset, buffer, count));
	open_fd.offset += n_read;
	return n_read;
}

FROG::ErrorOr<void> Process::creat(FROG::StringView path, mode_t mode) {
	LockGuard _(m_lock);

	auto absolute_path = TRY(absolute_path_of(path));
	while (absolute_path.sv().back() != '/')
		absolute_path.pop_back();
	auto parent_inode = TRY(VirtualFileSystem::get().file_from_absolute_path(absolute_path));
	if (path.count('/') > 0)
		return FROG::Error::from_c_string("You can only create files to current working directory");
	TRY(parent_inode.inode->create_file(path, mode));
	return {};
}

FROG::ErrorOr<void> Process::fstat(int fd, struct stat *out) {
	LockGuard _(m_lock);

	TRY(validate_fd(fd));
	const auto &open_fd = open_file_description(fd);
	out->st_dev = 0;
	out->st_ino = open_fd.inode->ino();
	out->st_mode = open_fd.inode->mode();
	out->st_nlink = open_fd.inode->nlink();
	out->st_uid = open_fd.inode->uid();
	out->st_gid = open_fd.inode->gid();
	out->st_rdev = 0;
	out->st_size = open_fd.inode->size();
	out->st_atim = open_fd.inode->atime();
	out->st_mtim = open_fd.inode->mtime();
	out->st_ctim = open_fd.inode->ctime();
	out->st_blksize = open_fd.inode->blksize();
	out->st_blocks = open_fd.inode->blocks();
	return {};
}
FROG::ErrorOr<void> Process::stat(FROG::StringView path, struct stat *out) {
	LockGuard _(m_lock);
	int       fd = TRY(open(path, O_RDONLY));
	auto      ret = fstat(fd, out);
	MUST(close(fd));
	return ret;
}
FROG::ErrorOr<FROG::Vector<FROG::String>> Process::read_directory_entries(int fd) {
	LockGuard _(m_lock);
	TRY(validate_fd(fd));
	auto &open_fd = open_file_description(fd);
	auto  result = TRY(open_fd.inode->read_directory_entries(open_fd.offset));
	open_fd.offset++;
	return result;
}

FROG::String Process::working_directory() const {
	LockGuard _(m_lock);
	return m_working_directory;
}

FROG::ErrorOr<void> Process::set_working_directory(FROG::StringView path) {
	LockGuard _(m_lock);

	FROG::String absolute_path = TRY(absolute_path_of(path));

	auto file = TRY(VirtualFileSystem::get().file_from_absolute_path(absolute_path));
	if (!file.inode->ifdir()) return FROG::Error::from_errno(ENOTDIR);

	m_working_directory = FROG::move(file.canonical_path);

	return {};
}

FROG::ErrorOr<FROG::String> Process::absolute_path_of(FROG::StringView path) const {
	LockGuard _(m_lock);

	if (path.empty()) return m_working_directory;
	FROG::String absolute_path;
	if (path.front() != '/') {
		TRY(absolute_path.append(m_working_directory));
		if (m_working_directory.sv().back() != '/') TRY(absolute_path.push_back('/'));
	}
	TRY(absolute_path.append(path));
	return absolute_path;
}

FROG::ErrorOr<void> Process::validate_fd(int fd) {
	LockGuard _(m_lock);

	if (fd < 0 || m_open_files.size() <= (size_t) fd || !m_open_files[fd].inode)
		return FROG::Error::from_errno(EBADF);
	return {};
}

Process::OpenFileDescription &Process::open_file_description(int fd) {
	LockGuard _(m_lock);

	MUST(validate_fd(fd));
	return m_open_files[fd];
}

FROG::ErrorOr<int> Process::get_free_fd() {
	LockGuard _(m_lock);

	for (size_t fd = 0; fd < m_open_files.size(); fd++)
		if (!m_open_files[fd].inode) return fd;
	TRY(m_open_files.push_back({}));
	return m_open_files.size() - 1;
}

} // namespace Kernel