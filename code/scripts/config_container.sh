echo 0 >/proc/sys/kernel/randomize_va_space
echo 256000000 >/sys/fs/cgroup/memory/lxc/test_container5/memory.limit_in_bytes
echo 256000000 >/sys/fs/cgroup/memory/lxc/test_container1/memory.limit_in_bytes
echo 256000000 >/sys/fs/cgroup/memory/lxc/test_container5/memory.soft_limit_in_bytes
echo 256000000 >/sys/fs/cgroup/memory/lxc/test_container1/memory.soft_limit_in_bytes
echo 256000000 >/sys/fs/cgroup/memory/lxc/test_container/memory.limit_in_bytes
echo 256000000 >/sys/fs/cgroup/memory/lxc/test_container/memory.soft_limit_in_bytes
echo 256000000 >/sys/fs/cgroup/memory/lxc/test_container2/memory.limit_in_bytes
echo 256000000 >/sys/fs/cgroup/memory/lxc/test_container2/memory.soft_limit_in_bytes
