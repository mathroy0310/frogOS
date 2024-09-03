/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:03:32 by maroy             #+#    #+#             */
/*   Updated: 2024/09/03 16:01:23 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/String.h>
#include <FROG/StringView.h>
#include <FROG/Vector.h>
#include <kernel/FS/Inode.h>
#include <kernel/Thread.h>

namespace Kernel {

class Process : FROG::RefCounted<Process> {
	FROG_NON_COPYABLE(Process);
	FROG_NON_MOVABLE(Process);

  public:
	using entry_t = Thread::entry_t;

  public:
	static FROG::ErrorOr<FROG::RefPtr<Process>> create_kernel(entry_t, void *);
	~Process() {}

	FROG::ErrorOr<void> add_thread(entry_t, void *);
	void                on_thread_exit(Thread &);

	pid_t pid() const { return m_pid; }

	FROG::ErrorOr<int>    open(FROG::StringView, int);
	FROG::ErrorOr<void>   close(int);
	FROG::ErrorOr<size_t> read(int, void *, size_t);

	FROG::StringView    working_directory() const { return m_working_directory; }
	FROG::ErrorOr<void> set_working_directory(FROG::StringView);

	Inode &inode_for_fd(int);

	static FROG::RefPtr<Process> current() { return Thread::current()->process(); }

  private:
	Process(pid_t pid) : m_pid(pid) {}

	FROG::ErrorOr<FROG::String> absolute_path_of(FROG::StringView) const;

  private:
	struct OpenFileDescription {
		FROG::RefPtr<Inode> inode;
		FROG::String        path;
		size_t             offset = 0;
		uint8_t            flags = 0;

		FROG::ErrorOr<size_t> read(void *, size_t);
	};

	FROG::ErrorOr<void>   validate_fd(int);
	OpenFileDescription &open_file_description(int);
	FROG::ErrorOr<int>    get_free_fd();

	FROG::Vector<OpenFileDescription> m_open_files;

	pid_t                              m_pid = 0;
	FROG::String                       m_working_directory;
	FROG::Vector<FROG::RefPtr<Thread>> m_threads;

	friend class FROG::RefPtr<Process>;
};

} // namespace Kernel